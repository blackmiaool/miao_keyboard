-- Don't use chinese comment.
local ahk_data={};
local config_string=read_file("config/config.txt");  

local shift_table_data=init_datasheet("config/shift_table.txt",1); 
local key_index_data=init_datasheet("config/key_index.txt",3); 
local ascii2usb_data=init_datasheet("config/ascii2usb.txt",3);

local two_mods={1,2,4,8,16,32,64,128,256};
local ahk_avaliable_function={
    restart= function() clear_key();restart_keyboard(); end,
}
local wait_release=false;
local key_map_index=1;

function ascii2usb(ascii)

    return read_datasheet(ascii2usb_data,ascii)
end
function shift_table(ascii)

    return read_datasheet(shift_table_data,ascii)
end
function key_index_table(pos,index)
    if not index then
        index=key_map_index;
    end

    local num=read_datasheet(key_index_data,pos+(index-1)*16*5);

    return num;
end

getmetatable('').__index = function(str,i) return string.sub(str,i,i) end
getmetatable('').__call = function(str,i,j)
    if type(i)~='table' then return string.sub(str,i,j)
    else local t={}
        for k,v in ipairs(i) do t[k]=string.sub(str,v,v) end
        return table.concat(t)
    end
end

function get_num_from_string(str,index,width)
    local num_str="";
    for i=1,width do
        num_str=num_str .. str[(index*(width+1))+i];
    end
    return tonumber(num_str)
end

function get_bit(num,index)
    return math.floor(num/two_mods[index]%2)==1;
end
function control_cmp(now,target)
    for i=1,4 do
        local tleft=get_bit(target,i);
        local nleft=get_bit(now,i);
        local tright=get_bit(target,i+4);        
        local nright=get_bit(now,i+4);
        local both=false;
        if tleft and tright then
            both=true;
        end
        if both then
            if not (nright or nleft) then
            return false
            end
        else
            if tleft and not nleft then
                return false
            end
            if tright and not nright then
                return false
            end
        end 
        if (nleft or nright) and not (tright or tleft) then
            return false;
        end
    end
    return true;
end

function set_key_map_index(index,led)
    key_map_index=index;
    if index~=1 and led then
        led_clear();
        led_set_mode(0);

        for i=0,13 do
            for j=0,4 do
                if key_index_table(j*16+i,index)~=key_index_table(j*16+i,1) then
                    led_set_bit(i,j,1);
                end
            end
        end
    else        
        led_clear();
        led_set_mode(1);
    end
end
set_key_map_index(1,true);
function key_input(c1,c2,k1,k2,k3,k4,k5,k6)

    local k={k1,k2,k3,k4,k5,k6};
    local keys={};
    local clear=false;
    local dont_clear=false;
    
    
    for i=1,c2 do
        if key_index_table(k[i],1)==135  then
        	dont_clear=true;
            set_key_map_index(2,true);
        elseif key_map_index==2 then
        	clear=true;
        	
        end
        if key_index_table(k[i],1)==136  then
            if key_map_index~=3 then
                set_key_map_index(3,true);
            else
                set_key_map_index(1,true);
            end
        end
    end

    if c2==0 and key_map_index==2 then
    	set_key_map_index(1,true); 
    end

    for i=1,6 do        
        if k[i] then
            keys[i]=key_index_table(k[i]);            
        else
            keys[i]=0;
        end                
    end

	if clear and not dont_clear then
    	output(0,0,0,0,0,0,0,0);
    	set_key_map_index(1,true);
    	return true;
    end

    if  c2==0 then
        wait_release=false;
    end

    if c1>0 and ahk_data[keys[1]] then
        for k,expression in pairs(ahk_data[keys[1]]) do
            if control_cmp(c1,k) then
                output_ahk(expression);
                wait_release=true;

                return true;
            end
            
        end
    end


    if not wait_release then
        output(c1,0,table.unpack(keys))
        return true;
    else
        return true;
    end

end


function ahk_parse(text_input)
    local control2val={
        ["^"]=1+16,["+"]=2+32,["!"]=4+64,["#"]=8+128,
        ["<^"]=1,["<+"]=2,["<!"]=4,["<#"]=8,
        [">^"]=16,[">+"]=32,[">!"]=64,[">#"]=128
    }


    for controls_text,key,expression in string.gmatch(text_input,"([%^%+!#<>]+)([%a%d])::(%g+)") do
        print(controls_text,key,expression.."\r\n") ;
        local controls=0;
        for control in string.gmatch(controls_text,"[<>]?[%^%+!#]") do
            controls = controls + control2val[control];
        end
        local key_index=ascii2usb(string.byte(key));
        if not ahk_data[key_index] then
            ahk_data[key_index]={}
        end
        ahk_data[key_index][controls]=expression;
        
    end
end


function match_sub_pattern(str,...)
    local output = {};
    local patterns={...};
    local current = 1;
    while pos ~= #str do
        local pos;
        local kind;
        for i,p in ipairs(patterns) do
            local l1,l2=string.find(str,p,current);
            if l1==current then
                pos=l2;
                kind=i;
                break;
            end
        end
        if pos then
            table.insert(output,{kind,string.sub(str,current,pos)})
            current=pos+1;
        else
            break;
        end
    end
    return output;
end
function clear_key()
        output(0,0,0,0,0,0,0,0)
end
function output_ahk(expression)

    local result= match_sub_pattern(expression,"[^{}]+","{[%a]+}","{[%a%G]+}");

    
    function press_string(str)
        clear_key();
        for i=1,#str do
            -- print(str[i])
            local byte=string.byte(str[i]);
            if shift_table(byte)==0 then
                output(0,0,ascii2usb(byte),0,0,0,0,0);
            else
                output(2,0,ascii2usb(byte),0,0,0,0,0);
            end
            
            clear_key();
            delay(30);
        end
    end
    function press_key(index)
        clear_key();
        output(0,0,index,0,0,0,0,0)

    end
    function change_key(index,action)
    end
    for i,sub_express in ipairs(result) do
        if sub_express[1]==1 then
            press_string(sub_express[2]);
        elseif sub_express[1]==2 then
            local str=string.sub(sub_express[2],2,-2);
            if ahk_avaliable_function[str] then
                ahk_avaliable_function[str]()
            else
                local index = get_key_index(str);
                if index>0 then
                    press_key(index);
                else 
                    if str=="leftbracket" then
                        output(2,0,47,0,0,0,0,0);
                        delay(30);
                        clear_key();
                        delay(30);
                    elseif str=="rightbracket" then
                        output(2,0,48,0,0,0,0,0);
                        delay(30);
                        clear_key();
                        delay(30);
                    end                   
                end                
            end                    
        elseif sub_express[1]==3 then

        end    
    end

end

ahk_parse(config_string);
config_string=nil;
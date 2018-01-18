-- Don't use chinese comment.
-- ahk's map ahk_data[normal_key][modifier_key]
local ahk_data={}; 
local ahk_file=read_file("config/config.txt");  

-- init_datasheet(file_name, data_width): lua parse the table file,return a table for lua
-- shift_table: (user shouldn't modify it) which key needs to be output with a shift key 
-- key_index: (most user may modify it) record 3 modes' usb value in a key map
-- asciiusb: (user shouldn't modify it) ascii value to usb value
local shift_table_data=init_datasheet("config/shift_table.txt",1); 
local key_index_data=init_datasheet("config/key_index.txt",3); 
local ascii2usb_data=init_datasheet("config/ascii2usb.txt",3);

-- count of rows and column in the "key_index.txt" file
local rows=5;
local columns=16;

-- for quick calc 
local two_mods={1,2,4,8,16,32,64,128,256};

-- functions that can be invoked in ahk file
local ahk_avaliable_function={
    restart= function() clear_key();restart_keyboard(); end,
}

-- mode2 and mode3's value in key_index.txt file
local mode2_key=135;
local mode3_key=136;
-- wait for user releasing all the key, do nothing before releasing
local wait_release=false;

-- currently using key map
local key_map_mode=1;

-- use to solve linux capslock delay bug
local capslockCode=57;

function ascii2usb(ascii)
    return read_datasheet(ascii2usb_data,ascii)
end

function shift_table(ascii)

    return read_datasheet(shift_table_data,ascii)
end

function get_key_from_position(pos,index)
    if not index then
        index=key_map_mode;
    end

    local num=read_datasheet(key_index_data,pos+(index-1)*columns*rows);

    return num;
end

-- extend original string class, now we can use brackets to get a sub string from a string like: a[2]
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

-- if the modifier keys satisfy the target one
function modifier_compare(modifier,target)
    for i=1,4 do
        local tleft=get_bit(target,i);
        local nleft=get_bit(modifier,i);
        local tright=get_bit(target,i+4);        
        local nright=get_bit(modifier,i+4);
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

function set_key_map_mode(mode,led)
    key_map_mode=mode;
    if mode~=1 and led then
        led_clear();
        led_set_mode(0);

        for i=0,13 do
            for j=0,4 do
                if get_key_from_position(j*16+i,mode)~=get_key_from_position(j*16+i,1) then
                    led_set_bit(i,j,1);
                end
            end
        end
    else        
        led_clear();
        led_set_mode(1);
    end
end
set_key_map_mode(1,true);

local previous_cnt=0;
local previous_key_map_mode=1;
local previous_capslock=false;
-- if k1==27 then
--     mouse_output(3,0xe9,0,0,0);
--     print("!!!");
--     return true;
-- end
local media_map={
    [127]=0xe2,
    [128]=0xe9,
    [129]=0xea
}
function media_output(value)
    mouse_output(3,value%256,math.floor(value/256),0,0);
end
local pressed_media=false;
local pressed_capslock=false;
function key_input_underlying(controls,cnt,key_arr)
    local final_normal_keys={};

    if key_map_mode==2 then
    	key_map_mode=1;
    end
    -- decide current key_map_mode
    for i=1,cnt do
        local value=get_key_from_position(key_arr[i],1);
        if value==mode2_key  then
            key_arr[i]=nil;
            set_key_map_mode(2,true); 	
        elseif value==mode3_key  then
            key_arr[i]=nil;
            if key_map_mode~=3 then
                set_key_map_mode(3,true);
            else
                set_key_map_mode(1,true);
            end
        elseif value == capslockCode then
            pressed_capslock=true;
            if pressed_capslock and previous_capslock then
                key_arr[i]=nil;
            end
        end        
    end

    -- handle mode2 race condition
    if key_map_mode ~=2 and previous_key_map_mode == 2 then
    	output(0,0,0,0,0,0,0,0);
    	return;
    end
    
    -- get final key values 
    local valid_key_cnt=0;

    for i=1,cnt do        
        if key_arr[i] ~= nil then
            local value=get_key_from_position(key_arr[i]);

            

            if media_map[value] then
                media_output(media_map[value]);
                pressed_media=true;
                return ;
            end
            final_normal_keys[valid_key_cnt+1]=value;            
            valid_key_cnt=valid_key_cnt+1;        
        end                
    end
    
   

    for i=valid_key_cnt,cnt do
        final_normal_keys[i+1]=0;
    end
    
    if pressed_media then
        media_output(0);
        pressed_media=false;
    end
    -- wait for releasing all normal keys
    if cnt==0 then
        wait_release=false;
    end

    if previous_cnt < cnt and controls>0 and ahk_data[final_normal_keys[1]] then
        for key_arr,expression in pairs(ahk_data[final_normal_keys[1]]) do
            if modifier_compare(controls,key_arr) then
                output_ahk(expression);
                wait_release=true;
                return;
            end
            
        end
    end

    if not wait_release then
        output(controls,0,table.unpack(final_normal_keys))
    end
end
-- return true to capture the input, prevent default handling
-- cnt: pressing normal keys' count
function key_input(controls,cnt,k1,k2,k3,k4,k5,k6)	
    local key_arr={k1,k2,k3,k4,k5,k6};    
    pressed_capslock=false;
    key_input_underlying(controls,cnt,key_arr);
    previous_cnt=cnt;
    previous_key_map_mode=key_map_mode;
    previous_capslock=pressed_capslock;
    return true;
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

ahk_parse(ahk_file);
ahk_file=nil;

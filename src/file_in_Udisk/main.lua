local config_string=[[
!<+a::blackmiaool
^!+a::qk333333
<+d::sll{space}l
>#w::{esc}
>#n::{down}
>#p::{up}
>#f::{right}
>#b::{left}
>#a::{home}
>#e::{end}
]]; 

local ahk_data={};
ascii2usb_data=[[
000,000,000,000,000,000,000,000,000,000,000,000,000,040,000,000,
000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
044,030,052,032,033,034,036,052,038,039,037,046,054,086,055,056,
039,030,031,032,033,034,035,036,037,038,051,051,054,046,055,056,
031,004,005,006,007,008,009,010,011,012,013,014,015,016,111,018,
019,020,021,022,023,024,025,026,027,028,029,000,000,000,000,000,
053,004,005,006,007,008,009,010,011,012,013,014,015,016,017,018,
019,020,021,022,023,024,025,026,027,028,029,000,050,000,053,000,]];

shift_table_data=[[
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,]];

key_index_data=[[
041,030,031,032,033,034,035,036,037,038,039,045,046,042,000,000,
043,020,026,008,021,023,028,024,012,018,019,047,048,049,000,000,
057,004,022,007,009,010,011,013,014,015,051,052,040,040,000,000,
225,225,029,027,006,025,005,017,016,054,055,056,000,229,000,000,
224,227,082,226,044,044,044,044,231,080,079,080,135,136,000,000,
]]
two_mods={1,2,4,8,16,32,64,128,256};
ascii2usb_data=string.gsub(ascii2usb_data,"\n","");
shift_table_data=string.gsub(shift_table_data,"\n","")
key_index_data=string.gsub(key_index_data,"\n","");

function ascii2usb(ascii)
    print("ascii",ascii,"\r\n");
    return get_num_from_string(ascii2usb_data,ascii,3);
end
function shift_table(ascii)
    return get_num_from_string(shift_table_data,ascii,1);
end
function key_index_table(pos)
    return get_num_from_string(key_index_data,pos,3);
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
        -- print("nnn",num_str,"nnn");
    end
    return tonumber(num_str)
end


-- print("bbb",shift_table(30));
-- print("bbbb",ascii2usb(45));

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
        -- print("aaaa",tleft,tright,nleft,nright,"aaaa\r\n")
        if tleft and tright then
            -- print("both true\r\n")
            both=true;
        end
        if both then
            if not (nright or nleft) then
            -- print("1111114")
            return false
            end
        else
            if tleft and not nleft then
                -- print("1111112")
                return false
            end
            if tright and not nright then
                -- print("1111113")
                return false
            end
        end 
        if (nleft or nright) and not (tright or tleft) then
            return false;
        end
    end
    return true;
end
local wait_release=false;
function key_input(c1,c2,k1,k2,k3,k4,k5,k6)
    -- print(c1,c2,k1);
    -- print(c1,c2,k1,k2,k3,k4,k5,k6);
    -- local c=c1;
    local k={k1,k2,k3,k4,k5,k6};
    local keys={};
    
    if c2 then
        for i=1,c2 do        
            table.insert(keys,key_index_table(k[i]))
        end
    end
    -- print("key:  ",c1,c2,keys[1],"\r\n");
    -- for k,control_target in pairs(ahk_data) do
    --         print("aa",k,control_target,"bbc");
    -- end
    if  c2==0 then
        wait_release=false;
    end
    if c1>0 and ahk_data[keys[1]] then
        for k,expression in pairs(ahk_data[keys[1]]) do
            if control_cmp(c1,k) then
                -- print(k,expression);
                output_ahk(expression);
                wait_release=true;
                return true;
            end
            
        end
    end
 
    return wait_release;
    -- if ahk_data[key] and ahk_data[c1][keys[1]] then
    --     print("capture");
    --     print(ahk_data[c1][keys[1]]);
    -- end
end
-- function key_input(c1,c2,k1,k2,k3,k4,k5,k6)
--     local c=c1;
--     local k={k1,k2,k3,k4,k5,k6};
--     if ahk_data[c] and ahk_data[c][k1] then
--         print(ahk_data[c][k1]);
--     end
-- end

function ahk_parse(text_input)
    local control2val={
        ["^"]=1+16,["+"]=2+32,["!"]=4+64,["#"]=8+128,
        ["<^"]=1,["<+"]=2,["<!"]=4,["<#"]=8,
        [">^"]=16,[">+"]=32,[">!"]=64,[">#"]=128
    }


    for controls_text,key,expression in string.gmatch(text_input,"([%^%+!#<>]+)(%a)::(%g+)") do
        print("cycle\r\n");
        print(controls_text,key,expression.."\r\n") ;


        local controls=0;
        for control in string.gmatch(controls_text,"[<>]?[%^%+!#]") do
            controls = controls + control2val[control];
        end
        print("controls",controls,ascii2usb(string.byte(key)),"\r\n");
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
            print(current,pos,kind);
            table.insert(output,{["kind"]=kind,["value"]=string.sub(str,current,pos)})
            print(output[#output].value);
            current=pos+1;
        else
            break;
        end
    end
    return output;
end
function output_ahk(expression)
    print(expression,"bbbb\r\n");
    local result= match_sub_pattern(expression,"[^{}]+","{[%a]+}","{[%a%G]+}");
    print("----------")
    function clear_key()
        output(0,0,0,0,0,0,0,0)
    end
    function press_string(str)
        clear_key();
        for i=1,#str do
            -- print(str[i])
            output(0,0,ascii2usb(string.byte(str[i])),0,0,0,0,0)
            clear_key();
            delay(10);
        end
    end
    function press_key(index)
        clear_key();
        output(0,0,index,0,0,0,0,0)
        -- clear_key();
    end
    function change_key(index,action)
    end
    for i,sub_express in ipairs(result) do
        if sub_express.kind==1 then
            press_string(sub_express.value);
        elseif sub_express.kind==2 then
            local str=string.sub(sub_express.value,2,-2);
            print("aaa",str,"aaa");
            press_key(get_key_index(str));            
        elseif sub_express.kind==3 then

        end    
    end
    -- print(table.unpack(result).kind);
end
-- key_input();
ahk_parse(config_string);
config_string=nil;
-- print("lllllllllll",get_key_index("deleteforward"));
-- expression_parse("ll(;l{space}kK{down shift}oo");

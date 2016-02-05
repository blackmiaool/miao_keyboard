local input=[[
^>+a::blackmiaool
<+b::slll
]]
function key_input(c1,c2,k1,k2,k3,k4,k5,k6)
local c=c1;
local k={k1,k2,k3,k4,k5,k6}; 
end
function ahk_parse(text_input)
-- print(string.match(text_input,"([%^%+!#])(%a)::(%g+)"))
for controls_text,key,expression in string.gmatch(text_input,"([%^%+!#<>]+)(%a)::(%g+)") do
print(controls_text,key,expression) ;

local key_unit={}
key_unit.controls={};
for control in string.gmatch(controls_text,"[<>]?.") do
table.insert(key_unit.controls,control);
end
print(table.unpack(key_unit.controls))

    key_unit.key=key;

    key_unit.expression=expression;
end	
end
function put_string( ... )
end
function put_key( ... )
end
function press_key( ... )
end
function up_key( ... )
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
function expression_parse(expression)
local result=table.unpack( match_sub_pattern(expression,"[^{}]+","{[%a]+}","{[%a%G]+}"));
print("----------")
print(table.unpack(result));

end
-- key_input(); 
-- ahk_parse(input);
expression_parse("ll(;l{space}kK{down shift}oo");
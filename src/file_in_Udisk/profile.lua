

--key_send
--mouse_send

--flag_set(flag,value)--bit0:keyboard enable bit1:mouse enable 

function mouse_handle(a)
--	print("key:",a[0],"\r\n");
end
function key_handle(data)
	if macro_flag then
		for i =1,10 do
			key_data[key_index]=data[i];
			key_index=key_index+1;
		end
	end
	print("   key  "..key_index.."\n");
end

--mouse_handle
function macro_play(data)  
	flag_set(2,0); 
    print("macro_play\n") ;
    local one=0;
    for i =1,key_index/10-1 do

    	key_put_pure(key_data[(i-1)*10+1],
    		key_data[(i-1)*10+2],
    		key_data[(i-1)*10+3],
    		key_data[(i-1)*10+4],
    		key_data[(i-1)*10+5],
    		key_data[(i-1)*10+6],
    		key_data[(i-1)*10+7],
    		key_data[(i-1)*10+8],
    		key_data[(i-1)*10+9],
    		key_data[(i-1)*10+10]
    		);
    	
	end
end 

function macro_end(data) 
	print("macro_end\n") ;
	flag_set(0,0); 
    
end 

function macro_start(data)  
	print("macro_start\n") ;
	if macro_flag then
		macro_flag=false;
		macro_end(data);
		return 
	else
		key_index=1;
		flag_set(0,1);--keyboard enable
		macro_flag=true;
	end

    
end 

--main
key_data={n=300};
key_index=1;
macro_flag=false;
event={key_handle,mouse_handle,macro_start,macro_play}
print("WERWERWERWER");
key_register(3--[[macro_start]],'o',"rctrl")
key_register(4--[[macro_start]],'i',"rctrl")
while 1 do 
	a={wait_event()};
	(event[a[10]])(a);
end 

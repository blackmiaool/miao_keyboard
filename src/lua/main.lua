local function set_pos() 
    local key_pos_col={'A1','A8'} ;
    local key_pos_row={'A0','B0'} ;
    local key_name={
        {'esc',     41,'1',     30, '2',    31,'3',32,'4',    33, '5',    34,'6', 35,'7',36,'8',37,'9',38,'0',39,'-',45,'=',46,'backspace',42},
        {'tab',     43,'q',     20, 'w',    26,'e',8, 'r',    21, 't',    23,'y', 28,'u',24,'i',12,'o',18,'p',19,'[',47,']',48,'\\',49},
        {'caps',    57,'a',     4,  's',    22,'d',7, 'f',    9,  'g',    10,'h', 11,'j',13,'k',14,'l',15,';',51,'\'',52,'enter',40},
        {'lshift',  225,'z',    29, 'x',    27,'c',6, 'v',    25, 'b',    5,'n',  17,'m',16,',',54,'.',55,'/',56,'rshift',229},
        {'lctrl',   224,'lwin',227, 'my1',  251,'my2',252,'lalt',226,'space',44,'rctrl',228,'ralt',230,'my3',253,'my4',254,'my5',255},
    }
    for i=1,#key_pos_col do 
        local k=key_pos_col[i];
        c_set_key_pos('col',i,k);
    end 
    for i=1,#key_pos_row do 
        local k=key_pos_row[i];
        c_set_key_pos('row',i,k); 
    end 
    for i=1,#key_name do 
        for j=1,#key_name[i] do 
            c_set_key_index(i,j,key_name[i][j*2]); 
        end 
    end 
end 

function main() 
    set_pos(); 
end 
main(); 


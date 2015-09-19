char *lua_string="local function set_pos() \
    local key_pos_col={'A1','A8'} ;\
    local key_pos_row={'A0','B0'} ;\
    local key_name={\
        {41,30},\
        {43,20},\
        {},\
        {},\
        {},\
    }\
    for i=1,#key_pos_col do \
        local k=key_pos_col[i];\
        c_set_key_pos('col',i,k);\
    end \
    for i=1,#key_pos_row do \
        local k=key_pos_row[i];\
        c_set_key_pos('row',i,k); \
    end \
end \
\
function main() \
    set_pos(); \
end \
main(); "  ;
//char *lua_string="c_set_key_pos('22')";

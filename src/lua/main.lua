local function set_pos()
    local key_pos_col={
        "A5","B11","C8","A0"
    }
    local key_pos_row={
        "B1","W31","C8","A0"
    }
    for i,k in ipairs(key_pos_col) do
        c_set_key_pos("col",i,k)
    end
    for i,k in ipairs(key_pos_col) do
        c_set_key_pos("row",i,k)
    end
end

function main()
    set_pos();
end
main();
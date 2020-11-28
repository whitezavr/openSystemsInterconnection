if $is_connected == $c_false exit

; отправляем запрос
buffer tmp_buf 4 $code_release_req 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

exit:



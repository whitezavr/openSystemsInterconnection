if $is_connected == $c_false exit

; отправляем запрос
buffer tmp_buf 8 $code_please_tokens 4 $token 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

exit:



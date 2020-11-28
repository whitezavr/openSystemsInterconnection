if $is_connected == $c_false exit

if $token_sync == $c_false get_sync_token

; отправляем запрос
buffer tmp_buf 4 $code_sync_minor 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
goto exit

; захватываем маркер синхронизации
get_sync_token:
buffer tmp_buf 8 $code_take_tokens 4 2 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
token_sync $c_true varset
settimer $timeout S_SYNC_MINOR

exit:


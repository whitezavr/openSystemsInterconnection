if $is_connected == $c_false exit

; отправляем запрос
buffer tmp_buf 8 $code_give_tokens 4 $token 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

; обновляем значения маркеров
unbufferit tmp_tokens 4 token
if ($tmp_tokens & $c_token_sync) == 0 skip_sync_off
token_sync $c_false varset
skip_sync_off:
if ($tmp_tokens & $c_token_data) == 0 skip_data_off
token_data $c_false varset
skip_data_off:

exit:


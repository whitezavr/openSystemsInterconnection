if $is_connected == $c_false exit

; устанавливаем требуемые значения маркеров
token_sync $c_false varset
token_data $c_false varset
unbufferit tmp_tokens 4 tmp_buf
if ($tmp_tokens != 2) &&  ($tmp_tokens != 4) skip_data_on
token_data $c_true varset
skip_data_on:
if ($tmp_tokens != 2) && ($tmp_tokens != 3) skip_sync_on
token_sync $c_true varset
skip_sync_on:

; отправляем подтверждение
buffer tmp_buf 8 $code_resync_resp 4 $token 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

exit:

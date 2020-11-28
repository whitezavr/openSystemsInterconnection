if $is_connected != $c_false exit

; получаем и запоминаем параметры соединения, отправляем подтверждение подключения, заводим цикл отправки данных
unbufferit conn_defense 4 conn_n_sync 4 quality
buffer tmp_buf 4+sizeof(quality) $code_connect_resp 4 $quality sizeof(quality)
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

conn_addr $address varset
is_connected $c_true varset
now_release_req $c_false varset
now_sync_major_req $c_false varset
now_release_resp $c_false varset
now_sync_major_resp $c_false varset

exit:


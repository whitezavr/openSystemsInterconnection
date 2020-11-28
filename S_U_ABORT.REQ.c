if $is_connected == $c_false exit

; отправляем запрос
buffer tmp_buf 4 $code_abort 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

; сообщаем о разрыве соединения и переходим в состояние "разъединен"
sendup S_P_ABORT.IND
is_connected $c_false varset

exit:

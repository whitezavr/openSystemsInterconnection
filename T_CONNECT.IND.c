if $is_connected != $c_false exit

; запоминаем адрес подключающейся системы и отправляем подтверждение подключения
conn_addr $address varset
eventdown T_CONNECT.RESP address $address

exit:


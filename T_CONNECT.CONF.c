; отправляем запрос на подключение
buffer tmp_buf 4+sizeof(tmp_quality)+sizeof(tmp_demand) $code_connect_req 4 $tmp_quality sizeof(tmp_quality) $tmp_demand sizeof(tmp_demand)
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf

exit:

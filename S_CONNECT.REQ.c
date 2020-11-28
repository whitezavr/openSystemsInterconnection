if $is_connected != $c_false exit

; запоминаем параметры покдлючения
conn_addr $address varset
tmp_quality $quality varset
tmp_demand $demand varset

; устанавливаем маркеры в нужное значение
token_sync $c_false varset
token_data $c_false varset
unbufferit tmp_tokens 4 tmp_demand
if ($tmp_tokens != 1) && ($tmp_tokens != 3) skip_data_on
	token_data $c_true varset

skip_data_on:
	if ($tmp_tokens != 1) && ($tmp_tokens != 4) skip_sync_on
		token_sync $c_true varset

skip_sync_on:
	; отправляем запрос на соединение
	eventdown T_CONNECT.REQ address $address
	eventdown T_CONNECT.REQ address $address
	eventdown T_CONNECT.REQ address $address
	eventdown T_CONNECT.REQ address $address
exit:



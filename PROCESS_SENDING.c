; основной процесс отправки пакетов в порядке очереди (считается, что все отправленные пакеты доставляются до адресата в период $timeout между отрабатываниями данного кода)
if $is_connected == $c_false clean
if (qcount(outgoingq) == 0) && ($now_release_req == $c_true) on_release_req
if (qcount(outgoingq) == 0) && ($now_release_resp == $c_true) on_release_resp
if (qcount(outgoingq) == 0) && ($now_sync_major_req == $c_true) on_syn_major_req
if (qcount(outgoingq) == 0) && ($now_sync_major_resp == $c_true) on_syn_major_resp
if qcount(outgoingq) == 0 exit
if $token_data == $c_false get_data_token

tmp_buf dequeue(outgoingq) varset
buffer tmp_buf 4+sizeof(tmp_buf) $code_data 4  $tmp_buf sizeof(tmp_buf)
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
if qcount(outgoingq) > 0 retimer
goto exit

; маркера данных нету и требуется отослать данные - отправляем сообщение о захвате маркера данных
get_data_token:
buffer tmp_buf 8 $code_take_tokens 4 1 4
token_data $c_true varset
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
goto retimer

; очередь пуста и производилось упорядоченное завершение соединения по нашему запросу - отправляем подтверждение наверх и рвем соединение
on_release_req:
sendup S_RELEASE.CONF
eventdown T_DISCONNECT.REQ address $conn_addr
is_connected $c_false varset
goto exit

; очередь пуста и производилось упорядоченное завершение соединения по чужому запросу - отправляем подтверждение другой системе
on_release_resp:
buffer tmp_buf 4 $code_release_resp 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
goto exit

; очередь пуста и производилась главная синхронизация по нашему запросу - отправляем подтверждение наверх
on_sync_major_req:
sendup S_SYNC_MAJOR.CONF
now_sync_major_req $c_false varset
goto exit

; очередь пуста и производилась главная синхронизация по чужому запросу - отправляем подтверждение другой системе
on_sync_major_resp:
buffer tmp_buf 4 $code_sync_major_resp 4
eventdown T_DATA.REQ address $conn_addr userdata $tmp_buf
now_sync_major_resp $c_false varset
goto exit

; заново взводим таймер
retimer:
settimer $timeout PROCESS_SENDING timer_process_sending
goto exit

clean:
if qcount(outgoingq) == 0 exit
tmp_buf dequeue(outgoingq) varset
goto clean

exit:

; получаем: код назначения пакета, сами данные
unbufferit tmp_code 4 tmp_buf sizeof(userdata)-4 userdata

if $tmp_code == $code_connect_req on_connect_req
if $tmp_code == $code_connect_resp on_connect_resp
if $tmp_code == $code_release_req on_release_req
if $tmp_code == $code_release_resp on_release_resp
if $tmp_code == $code_abort on_abort
if $tmp_code == $code_resync_req on_resync_req
if $tmp_code == $code_resync_resp on_resync_resp
if $tmp_code == $code_data on_data
if $tmp_code == $code_sync_minor on_sync_minor
if $tmp_code == $code_sync_major_req on_sync_major_req
if $tmp_code == $code_sync_major_resp on_sync_major_resp
if $tmp_code == $code_please_tokens on_please_tokens
if $tmp_code == $code_give_tokens on_give_tokens
if $tmp_code == $code_take_tokens on_take_tokens
goto exit

; пакет запроса на соединение
on_connect_req:
if $is_connected != $c_false exit
unbufferit tmp_quality 8 tmp_demand 4 tmp_buf

; устанавливаем нужные значения маркеров
token_sync $c_false varset
token_data $c_false varset
unbufferit tmp_tokens 4 tmp_buf
if ($tmp_tokens != 2) && ($tmp_tokens != 4) skip_data_on
token_data $c_true varset
skip_data_on:
if ($tmp_tokens != 2) && ($tmp_tokens != 3) skip_sync_on
token_sync $c_true varset
skip_sync_on:

; извещаем уровень представления о запросе на подключение
sendup address $conn_addr quality $tmp_quality demand $tmp_demand S_CONNECT.IND

goto exit

; пакет подтверждения соединения
on_connect_resp:
if $is_connected != $c_false exit

unbufferit conn_defense 4 conn_n_sync 4 tmp_buf

; извещаем уровень представления
sendup address $conn_addr quality $tmp_buf S_CONNECT.CONF

; устанавливаем флаги в нужные значения
is_connected $c_true varset
now_release_req $c_false varset
now_sync_major_req $c_false varset
now_release_resp $c_false varset
now_sync_major_resp $c_false varset
must_sync_minor $c_false varset
must_sync_major $c_false varset

goto exit

; пакет запроса на упорядоченный разрыв соединения
on_release_req:
if $is_connected == $c_false exit
; извещаем уровень представления
sendup S_RELEASE.IND

goto exit

; пакет подтверждения упорядоченного разрыва соединения
on_release_resp:

if $is_connected == $c_false exit
now_release_resp $c_true varset

goto exit

; пакет запроса безусловного разрыва соединения
on_abort:
if $is_connected == $c_false exit
; извещаем уровень представления и рвем соединение
sendup S_P_ABORT.IND
eventdown T_DISCONNECT.REQ address $conn_addr
is_connected $c_false varset

goto exit

; пакет запроса ресинхронизации
on_resync_req:
if $is_connected == $c_false exit
; извещаем уровень представления (маркеры не меняем - возможно, наш уровень представления не подтвердит ресинхронизацию)
sendup token $tmp_buf S_RESYNCHRONIZE.IND

goto exit

; пакет подтверждения ресинхронизации
on_resync_resp:
if $is_connected == $c_false exit

; устанавливаем нужные значения маркеров
unbufferit tmp_tokens 4 tmp_buf
token_sync $tmp_tokens & $c_token_sync varset
token_data $tmp_tokens & $c_token_data varset
; отправляем наверх подтверждение ресинхронизации
sendup token $tmp_buf S_RESYNCHRONIZE.CONF

goto exit

; пакет с данными
on_data:
if $is_connected == $c_false exit
; отправляем выше на уровень представления
sendup userdata $tmp_buf S_DATA.IND

goto exit

; пакет с запросом малой синхронизации
on_sync_minor:
if $is_connected == $c_false exit
; отправляем выше на уровень представленияsendup S_SYNC_MINOR.IND

goto exit

; пакет с запросом главной синхронизации
on_sync_major_req:
if $is_connected == $c_false exit
; отправляем выше на уровень представления
sendup S_SYNC_MAJOR.IND

goto exit

; пакет с подтверждением главной синхронизации
on_sync_major_resp:
if $is_connected == $c_false exit
; отправляем выше на уровень представления
sendup S_SYNC_MAJOR.CONF

goto exit

; пакет с запросом маркеров данных и/или синхронизации
on_please_tokens:
if $is_connected == $c_false exit
; отправляем выше на уровень представления
sendup token $tmp_buf S_PLEASE_TOKENS.IND

goto exit

; пакет передающий маркер данных и/или синхронизации
on_give_tokens:
if $is_connected == $c_false exit

; устанавливаем нужные значения маркеров
unbufferit tmp_tokens 4 tmp_buf
token_sync ($tmp_tokens & $c_token_sync) | $token_sync varset
token_data ($tmp_tokens & $c_token_data) | $token_data varset
; извещаем уровень представления
sendup token $tmp_buf S_GIVE_TOKENS.IND

; если было необходимо отправить какие-либо данные, вызываем обработчик PROCESS_SENDING
if (qcount(outgoingq) == 0) || ($token_data == $c_false) skip_send_data
settimer 0 PROCESS_SENDING timer_process_sending
skip_send_data:

goto exit

; извещение о захвате маркера
on_take_tokens:
if $is_connected == $c_false exit

; обновляем значения маркеров
unbufferit tmp_tokens 4 tmp_buf
if ($tmp_tokens & $c_token_sync) == 0 skip_sync_off
token_sync $c_false varset
skip_sync_off:
if ($tmp_tokens & $c_token_data) == 0 skip_data_off
token_data $c_false varset
skip_data_off:

goto exit

exit:

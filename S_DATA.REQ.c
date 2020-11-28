if $is_connected == $c_false exit

; помещаем нужный буффер в очередь отправки
queueit outgoingq $userdata

if qcount(outgoingq) == 1 call_now
; взвести таймер на $timeout
settimer $timeout PROCESS_SENDING timer_process_sending
goto exit

call_now:
settimer 1 PROCESS_SENDING timer_process_sending

exit:


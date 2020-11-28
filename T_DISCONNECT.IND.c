if $is_connected == $c_false exit
if $now_release_req == $c_true conf_disconn
if $now_release_resp == $c_true connect_off

sendup S_P_ABORT.IND
goto connect_off

conf_disconn:
sendup S_RELEASE.CONF
goto connect_off

connect_off:
is_connected $c_false varset

exit:


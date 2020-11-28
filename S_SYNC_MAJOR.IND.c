if $numbdp != $numResp resync
      eventdown T_DATA.REQ userdata $sync_major 
                   rub dequeue(senddata) varset 

resync:  
     settimer nSync $tok 0 S_RESYNCRONIZE.REQ t0

end:

diag_log "---------------------------------------------------------------------";
private _result = "overmind_server" callExtension "VERSION";
if (_result == "") then {
    diag_log "Overmind Server: failed to load";
    overmind_server_var_loaded = false;
} else {
    diag_log "Overmind Server: loaded";
    overmind_server_var_loaded = true;
};
diag_log "---------------------------------------------------------------------";
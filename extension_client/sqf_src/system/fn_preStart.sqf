diag_log "---------------------------------------------------------------------";
private _result = "overmind_client" callExtension "VERSION";
if (_result == "") then {
    diag_log "Overmind Client: failed to load";
    overmind_client_var_loaded = false;
} else {
    diag_log "Overmind Client: loaded";
    overmind_client_var_loaded = true;
};
diag_log "---------------------------------------------------------------------";
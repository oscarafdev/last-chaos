Control
 Name: TTRS Turn Left
 Control1: Mouse X-
 Device1: Mouse
 Hidden: TRUE
 Held: ctl_fTurnLeft = ctl_fTurnLeft+ctl_fInputValue;


Control
 Name: TTRS Turn Right
 Control1: Mouse X+
 Device1: Mouse
 Hidden: TRUE
 Held: ctl_fTurnRight = ctl_fTurnRight+ctl_fInputValue;


Control
 Name: TTRS Look Up
 Control1: Mouse Y+
 Device1: Mouse
 Hidden: TRUE
 Held: ctl_fTurnUp = ctl_fTurnUp+ctl_fInputValue;


Control
 Name: TTRS Look Down
 Control1: Mouse Y-
 Device1: Mouse
 Hidden: TRUE
 Held: ctl_fTurnDown = ctl_fTurnDown+ctl_fInputValue;


Control
 Name: TTRS Up/Jump
 Control1: Space
 Device1: Keyboard
 Released: ctl_fMoveUp = 0;
 Held: ctl_fMoveUp = ctl_fMoveUp + ctl_fInputValue;


GlobalInvertLook
GlobalSmoothAxes
GlobalSensitivity 52

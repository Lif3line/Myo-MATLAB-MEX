function [data] = timerCallback(obj,~)
    [emgData,timeNow] = myoInterface(0);
    data = {emgData,timeNow};
    set(obj,'UserData',data);
end
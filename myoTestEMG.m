%% Pseudo-real-time EMG data extraction from Myo to MATLAB

clear; clc; close all % House keep

%% Genmeral Setup
sampMax = 40;

myoInterface(1); % Set up myo connection in mex file (which is a DLL)

% Timer code: essentially run a loop to get EMG data into the mex file
% (which operates as a DLL)
t = timer('StartDelay',0,'Period',0.004,'ExecutionMode','fixedRate', 'BusyMode', 'drop');
t.TimerFcn  = {@timerCallback};
start(t);

figure(1) % Figure for showing EMG
set(gcf,'currentchar',']') % Used for exit on button press

while get(gcf,'currentchar') == ']' % While no button (other than ]) has been pressed
    data = get(t,'UserData'); % Get data from tiemr function (call as often as needed: holds last 40 samples ~200ms)
    
    emgData = reshape(data{1},[8,40])'; % Reshape data so columns are channels and rows are samples
    timestamps = data{2}; % Timestamps for each cycle
    
    plot(emgData)
    ylim([-128 127])
    xlim([1 sampMax])
    title('Myo EMG Data')
    xlabel('Samples')
    ylabel('Amplitude')
    drawnow
    pause(0.2); % Arbitary pause for new data to accumulate
end

% Clean up: remove timer object, unload mex etc
stop(t);
delete(t);
close(gcf)
clear mex 

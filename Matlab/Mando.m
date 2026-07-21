function Mando
global CMD
% Inicialización: [Throttle %; Roll %; Pitch %; Yaw %]
% Ponemos el acelerador en 47.4% (hover aproximado) y el resto en 0%
if isempty(CMD), CMD = [47.4; 0; 0; 0]; end

f = figure(100); clf(f);
set(f,'Name','MANDO KE88 (Porcentajes)','Color','w','NumberTitle','off');
txt = uicontrol(f,'Style','text','Units','normalized',...
    'Position',[0.05 0.05 0.9 0.9],'FontSize',13,'FontName','FixedWidth',...
    'HorizontalAlignment','left','BackgroundColor','w');
set(f,'KeyPressFcn',@tecla,'KeyReleaseFcn',@suelta);
refresca();

    function tecla(~,e)
        paso_gas = 2.0;    % Cuánto sube/baja el gas por pulsación (%)
        paso_maniobra = 100; % Porcentaje de inclinación al pulsar una flecha (%)

        switch e.Key
            % --- THROTTLE (0% a 100%) ---
            case 'w',          CMD(1) = min(100, CMD(1) + paso_gas);
            case 's',          CMD(1) = max(0,   CMD(1) - paso_gas);

                % --- PITCH (Eje longitudinal: Flechas Arriba/Abajo) ---
            case 'uparrow',    CMD(3) = min(100, CMD(3) + paso_gas);
            case 'downarrow',  CMD(3) = max(-100, CMD(3) - paso_gas);

                % --- ROLL (Eje lateral: Flechas Derecha/Izquierda) ---
            case 'rightarrow', CMD(2) = min(100, CMD(2) + paso_gas);
            case 'leftarrow',  CMD(2) = max(-100, CMD(2) - paso_gas);
%% 

                % --- YAW (Giro sobre eje Z: A/D) ---
            case 'a',          CMD(4) = -paso_maniobra;
            case 'd',          CMD(4) =  paso_maniobra;

                % --- UTILIDADES ---
            case 'space',      CMD(2:4) = 0; % Nivelar mandos (Roll, Pitch, Yaw a 0%)
            case 'r',          CMD(1) = 50; % Resetear gas a hover (50%)
        end
        refresca();
    end

    function suelta(~,e)
        % Resetea el porcentaje al soltar la tecla (comportamiento autocentrado del Joystick)
        switch e.Key
            case {'uparrow','downarrow'},    CMD(3) = 0;
            case {'leftarrow','rightarrow'}, CMD(2) = 0;
            case {'a','d'},                  CMD(4) = 0;
        end
        refresca();
    end

    function refresca()
        set(txt,'String',sprintf(['   MANDO KE88 (Porcentajes - deja el foco aqui)\n\n',...
            '   Throttle: %5.1f %%\n   Roll:     %5.1f %%\n',...
            '   Pitch:    %5.1f %%\n   Yaw:      %5.1f %%\n\n',...
            '   W/S gas   R hover   flechas mover\n',...
            '   A/D girar   ESPACIO nivelar'], CMD));
    end
end
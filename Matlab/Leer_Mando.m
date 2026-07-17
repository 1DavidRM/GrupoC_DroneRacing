function y = Leer_Mando(~)
% Declaramos la variable global donde la ventana guarda las teclas
global CMD

% Si la ventana de teclas no se ha abierto aún, inicializamos el mando
% con el acelerador en 47.4% (hover) y el resto en 0%
if isempty(CMD)
    CMD = [47.4; 0; 0; 0]; 
end

% Enviamos a Simulink el vector de 4 elementos: [Throttle; Roll; Pitch; Yaw]
y = CMD;
end
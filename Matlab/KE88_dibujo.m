function y = KE88_dibujo(x)
% KE88_DIBUJO Dibuja el dron en 3D a partir de un vector de estado de 12 variables (x_12)
% x(1:3) -> Posición Xe, Ye, Ze
% x(7:9) -> Ángulos de Euler phi, theta, psi (Roll, Pitch, Yaw en radianes)

persistent ax hT tr X t0
y = 0; 
x = x(:); % Aseguramos que sea un vector columna

% ---------- Parámetros de ajuste del modelo STL ----------
% Si el modelo se ve "de cabeza" o el frente apunta a otro lado, se ajusta aquí.
ESCALA_STL       = 0.0017;
ROLL_OFFSET_DEG  = 90;   % Corrige que el dron vuele de espaldas
PITCH_OFFSET_DEG = 0;     
YAW_OFFSET_DEG   = 0;    % Corrige el frente del dron

if isempty(ax) || ~isgraphics(ax)
    % 1. Crear ventana gráfica limpia
    f = figure(99); clf(f); 
    set(f, 'Name', 'KE88 en vivo (STL)', 'Color', 'w');
    ax = axes('Parent', f); 
    hold(ax, 'on'); 
    grid(ax, 'on'); 
    view(ax, 45, 20);
    axis(ax, 'equal');
    
    % 2. Dibujar el plano del suelo en Z = 0
    [gx, gy] = meshgrid(-3:1:3);
    surf(ax, gx, gy, 0*gx, 'FaceColor', [0.9 0.95 0.9], 'EdgeColor', [0.8 0.85 0.8]);
 
    % 3. Cargar el modelo STL (Una sola vez para optimizar rendimiento)
    ruta_stl = fullfile(fileparts(mfilename('fullpath')), 'Dron_Equipo_C.STL');
    if ~exist(ruta_stl, 'file')
        error('No se encontró el archivo STL "Dron_Equipo_A.STL" en la carpeta del script.');
    end
    
    TR = stlread(ruta_stl);
    V  = TR.Points;                 % Vértices nativos del STL
    F  = TR.ConnectivityList;       % Conectividad de caras
 
    % Centrar el modelo en su origen de gravedad
    c = (max(V,[],1) + min(V,[],1)) / 2;
    V = V - c;
 
    Vb = V; % Copia para aplicar rotaciones de ajuste
 
    % Matrices de rotación de ajuste (Grados a Radianes)
    cr = cosd(ROLL_OFFSET_DEG);  sr = sind(ROLL_OFFSET_DEG);
    cp = cosd(PITCH_OFFSET_DEG); sp = sind(PITCH_OFFSET_DEG);
    cy = cosd(YAW_OFFSET_DEG);   sy = sind(YAW_OFFSET_DEG);
    
    Rx_ = [1 0 0; 0 cr -sr; 0 sr cr];
    Ry_ = [cp 0 sp; 0 1 0; -sp 0 cp];
    Rz_ = [cy -sy 0; sy cy 0; 0 0 1];
    
    R_ajuste = Rz_ * Ry_ * Rx_;
    Vb = (R_ajuste * Vb')';
 
    % 4. Escalado automático o manual
    global P
    if isempty(P)
        % Si no tienes cargados los parámetros de hélice, usa la escala por defecto
        escala = ESCALA_STL;
    else
        % Si existen, calcula el tamaño dinámico real según el chasis
        diag_real = 2 * sqrt(P.dx^2 + P.dy^2);
        span      = Vb(:, [1 2]);
        diag_stl  = norm(max(span,[],1) - min(span,[],1));
        escala    = diag_real / diag_stl;
    end
    Vb = Vb * escala;
 
    % 5. Crear el nodo de transformación de hardware para GPU
    hT = hgtransform('Parent', ax);
    patch('Parent', hT, 'Faces', F, 'Vertices', Vb, ...
        'FaceColor', [0.25 0.55 0.85], 'EdgeColor', 'none', ...
        'FaceLighting', 'gouraud', 'AmbientStrength', 0.4);
 
    camlight(ax, 'headlight'); 
    lighting(ax, 'gouraud'); 
    material(ax, 'dull');
 
    % Línea de estela de la trayectoria
    tr = plot3(ax, nan, nan, nan, 'b-');
    xlabel(ax, 'x [m]'); ylabel(ax, 'y [m]'); zlabel(ax, 'z [m]');
    X = []; 
    t0 = tic;
end                       

% --- Control de FPS (Dibuja cada 0.08 segundos para no saturar MATLAB) ---
if toc(t0) < 0.08
    return; 
end
t0 = tic;
 
% --- Leer variables de estado desde el vector de 12 elementos ---
pos = x(1:3);             % Posiciones X, Y, Z (Elementos 1, 2 y 3)
pos(3) = max(pos(3), 0);  % Clamping para evitar que pase el suelo

ph = x(7);                % Roll (Elemento 7)
th = x(8);                % Pitch (Elemento 8)
ps = x(9);                % Yaw (Elemento 9)
 
% Matriz de rotación estándar de la Tierra al cuerpo usando ángulos de Euler
R = [cos(ps)*cos(th), cos(ps)*sin(th)*sin(ph)-sin(ps)*cos(ph), cos(ps)*sin(th)*cos(ph)+sin(ps)*sin(ph);
     sin(ps)*cos(th), sin(ps)*sin(th)*sin(ph)+cos(ps)*cos(ph), sin(ps)*sin(th)*cos(ph)-cos(ps)*sin(ph);
     -sin(th),        cos(th)*sin(ph),                         cos(th)*cos(ph)];
 
% Crear la matriz de transformación homogénea de 4x4
Tm = eye(4); 
Tm(1:3, 1:3) = R;   % Inserta la rotación
Tm(1:3, 4)   = pos; % Inserta la traslación

% Mover el modelo 3D por hardware
set(hT, 'Matrix', Tm);
 
% Guardar rastro de la estela (últimos 400 puntos)
X(:, end+1) = pos;
if size(X, 2) > 400
    X = X(:, 2:end); 
end
set(tr, 'XData', X(1,:), 'YData', X(2,:), 'ZData', max(X(3,:), 0));
 
% Hacer que la cámara siga al dron dinámicamente con margen de seguridad
margen = 1.5;
axis(ax, [pos(1)-margen, pos(1)+margen, ... % Rango X
          pos(2)-margen, pos(2)+margen, ... % Rango Y
          pos(3)-margen, pos(3)+margen]);   % Rango Z (Sigue dinámicamente al dron)
title(ax, sprintf('KE88 en vivo (STL)  |  Altura Z = %.2f m', pos(3)));

drawnow limitrate
end
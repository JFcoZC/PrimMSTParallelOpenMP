%Read results from file
fid = fopen('resultados.txt');
datos = textscan(fid, '%d %f');

%Datos originales
vertices = datos{1};
tiempo = datos{2};

%Promedio datos
verticesPromedio = [];
tiempoPromedio = [];

%Guardar el promedio de tiempo para vertices iguales
for i = 1:(size(vertices)-1)
    verticesPromedio(end+1) = (vertices(i)+vertices(i+1))/2
    tiempoPromedio(end+1) = (tiempo(i)+tiempo(i+1))/2
end%Fin for
    
%Ploting
line(vertices,tiempo);
ylabel('Time (seconds)');
xlabel('Number of processors');

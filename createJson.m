[v, L, ct] = Read_Brain_Annotation('lh.aparc.a2009s.annot');

data.coord = [0, 0, 0];
dataArray = struct('VisuOrder', {}, 'coord', {}, 'MatrixRow', {}, 'name', {}, 'VisuHierarchy', {}, 'labelValue', {}, 'AAL_ID', {}, 'label', {});
lsize = ct.numEntries;
map = []; %map from label to AAL_ID. We will need this later for rewriting vtk.

for i=1:lsize
    data.MatrixRow = i;
    data.VisuOrder = i;
    data.AAL_ID = i;
    data.name = ct.struct_names(i);
    data.VisuHierarchy = 'seed.left';
    data.labelValue = [ct.table(i, 1), ct.table(i, 2), ct.table(i, 3)];
    data.label = ct.table(i, 5);
    dataArray(i) = data;
    map = [map; data.label, data.AAL_ID];
end

[v, L, ct] = Read_Brain_Annotation('rh.aparc.a2009s.annot');
for i=1:ct.numEntries
    data.MatrixRow = i + lsize;
    data.VisuOrder = i + lsize;
    data.AAL_ID = i + lsize;
    data.name = ct.struct_names(i);
    data.VisuHierarchy = 'seed.right';
    data.labelValue = [ct.table(i, 1), ct.table(i, 2), ct.table(i, 3)];
    data.label = ct.table(i, 5);
    dataArray(i + lsize) = data;
    map = [map; data.label, data.AAL_ID];
end

jsonObject = json.dump(dataArray);
json.write(jsonObject, 'out.json');

% Write csv which we will need later to update vtk point data
csvwrite('out.csv', map);
[v, L, ct] = Read_Brain_Annotation('lh.aparc.a2009s.annot');

data.coord = [0, 0, 0];
dataArray = struct('VisuOrder', {}, 'coord', {}, 'MatrixRow', {}, 'name', {}, 'VisuHierarchy', {}, 'labelValue', {}, 'AAL_ID', {});
lsize = ct.numEntries;
for i=1:lsize
    data.MatrixRow = i;
    data.VisuOrder = i;
    data.AAL_ID = i;
    data.name = ct.struct_names(i);
    data.VisuHierarchy = 'seed.left';
    data.labelValue = [ct.table(i, 1), ct.table(i, 2), ct.table(i, 3)];
    dataArray(i) = data;
end

[v, L, ct] = Read_Brain_Annotation('rh.aparc.a2009s.annot');
for i=1:ct.numEntries
    data.MatrixRow = i + lsize;
    data.VisuOrder = i + lsize;
    data.AAL_ID = i + lsize;
    data.name = ct.struct_names(i);
    data.VisuHierarchy = 'seed.right';
    data.labelValue = [ct.table(i, 1), ct.table(i, 2), ct.table(i, 3)];
    dataArray(i + lsize) = data;
end

jsonObject = json.dump(dataArray);
json.write(jsonObject, 'out.json');
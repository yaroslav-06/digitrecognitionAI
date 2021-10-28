const express = require("express");
const path = require("path");

const app = express();

app.use('**/static', express.static(path.resolve(__dirname + '/build/static')));
app.use('**/st', express.static(path.resolve(__dirname + '/build/static')));
app.use('**/ai_data', express.static(path.resolve(__dirname + '/ai_data')));
app.use('**/cpp', express.static(path.resolve(__dirname + '/cpp')));

app.get('/*', (req, res)=>{
    res.sendFile(path.resolve(__dirname, 'build', 'index.html'));
});

app.listen(process.env.PORT || 5070, ()=>console.log('Server running...'));
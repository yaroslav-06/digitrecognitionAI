console.log("Hello, world!")

let mBox = document.querySelector("#mainBox")
let drawing = document.querySelector("#drawing")
let realN = document.querySelector("#realN")
let ansN = document.querySelector("#ansN")

let image: number[][] = []; //size 28*28
let img_el: Element[][] = []; //size 28*28

let n: number = 4;
let sz: number[] = [784, 16, 16, 10];

let biases: number[][] = [];
let weights: number[][][] = [];

let data, phNum: number = 0;

async function getData(){
    data = (await(await (await fetch("cpp/outDrawing")).text()).split('\n'))
    for (let i = 1; i < data.length; i++) {
        data[i]=data[i].split(',')
    }
    printPhoto(data, 0)
}

readTextFile("ai_data/ai");

function readTextFile(file){
    let rawFile = new XMLHttpRequest();
    rawFile.open("GET", file);
    rawFile.responseType = "arraybuffer";
    rawFile.onloadend = function(oEvent){
        let res = new Int32Array(rawFile.response);
        let curI: number = 0;
        for(let i = 0; i < n - 1; i++){
            weights[i] = [];
            biases[i] = [];
            for(let j = 0; j < sz[i]; j++){
                biases[i][j] = res[curI];
                curI++;
                weights[i][j] = [];
                for(let k = 0; k < sz[i + 1]; k++)
                    weights[i][j][k] = res[curI],
                    curI++;
            }
        }
        for(let i = 0; i < sz[n - 1]; i++)
            biases[n - 1] = [],
            biases[n - 1][i] = res[curI],
            curI++;
    }//*/
    console.log("biases: ", biases);
    console.log("weights: ", weights);
    rawFile.send(null);
}

function photoReses(): number{
    let ans: number;

    return ans;
}

(window as any).printNP = function pnPh(){
    phNum++;
    console.log('printing photo#',phNum)
    printPhoto(data, phNum)
};

(window as any).printPP = function ppPh(){
    phNum--
    console.log('printing photo#',phNum)
    printPhoto(data, phNum)
};

function printPhoto(dT, phN){
    let drSt="";
    //realN.innerHTML = "n: "+data[phN*30];
    /*for(let i=1+phN*30; i<29+phN*30; i++){
        drSt += "<div class='row'>";
        for(let j=0; j<28; j++)
            drSt += '<div style="height: calc(var(--b-size) / 28); width: calc(var(--b-size) / 28); background-color: rgba('+dT[i][j]+','+dT[i][j]+','+dT[i][j]+',1)"></div>\n'
        drSt += "</div>"
    }
    ansN.innerHTML=''
    for(let i=0; i<10; i++){
        ansN.innerHTML += "<div class='anses'><div class='dot', style='background-color: rgb(" + Math.round(255 * data[29 + 30 * phN][i]) + ',' + Math.round(255 * data[29 + 30 * phN][i]) + ',' + Math.round(255 * data[29 + 30 * phN][i]) + ")'>" + i + "</div>" + '<div style = "position: absolute; left: 24px;">' + data[29 + 30 * phN][i] + '</div></div>'
    }
    drawing.innerHTML = drSt*/
}

function createImage(){ // creating black screen
    let ans = "";
}

function displayPhoto(){
    let ans = "";
}

getData();
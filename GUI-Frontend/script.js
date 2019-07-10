var Binance = require('binance-api-node');
var client = Binance()

document.body.onload = init;

async function getPrices() {
    var prices = (await client.candles({ symbol: 'BTCUSDT', limit: 5}))
    console.log(prices);
}

function init() {

    var data = {
        labels: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri'],
        series: [
            [getPrices().prices[0].close, 2, 4, 2, 0]
        ]
    };
    
    var options = {
        width: 300, 
        height: 200
    };
    
    var newDiv = document.createElement("div");
    var newContent = new Chartist.Line('.ct-chart', data, options);
    newDiv.appendChild(newContent);

    var currentDiv = document.getElementById("chart1");
    document.body.insertBefore(newDiv, currentDiv)

    //new Chartist.Line('.ct-chart', data, options);       
}
init()
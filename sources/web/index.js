require = require('@std/esm')(module, { cjs: true, esm: 'js' });

var express    = require('express');        // call express
var app        = express();                 // define our app using express
var bodyParser = require('body-parser');

var catFeederFactory=require('./cat-feeder').CatFeederFactory;

    console.log('Starting CatFeeder server application...');
    var cat = catFeederFactory.create();
    cat.startService();

    

    app.use(bodyParser.urlencoded({ extended: true }));
    app.use(bodyParser.json());
    
    var port = process.env.PORT || 8000;        // set our port
    
    var router = express.Router();
    
    function httpMessage(code, message) {
        return {
            code: code,
            message: message
        };
    }
    
    var messageOk = httpMessage(200, 'OK');
    
    function send(res, code, message) {
        res.json(httpMessage(code, message)).sendStatus(code);
    }
    
    
    
    router.get('/', (req, res) => {
        res.json({ message: 'Welcome to the Feed The Cat API !' });   
    });
    
    router.get('/command', (req, res) => {
        cat.getCommand()
        .then(data => res.send({command: data}));
    });
    
    router.get('/calibration', (req, res) => {
        cat.getCalibration()
        .then(data => res.send({calibration: data}));
    });
    
    router.post('/calibration', (req, res) => {
        if (!req.body.value) {
            res.sendStatus(400);
        } else {
            cat.setCalibration(req.body.value).then(
                () => res.send(messageOk),
                () => err => send(res, 500, err));
        }
    })
    
    router.get('/infos', (req, res) => {
        cat.getInfos()
            .then(data => res.send(data), err => send(res, 500, err));
    });
    
    router.post('/feed', (req, res) => {
        if (!req.body.requestedFood) {
            res.sendStatus(400);
        } else {
            cat.feed(req.body.requestedFood)
            .then(() => res.send(messageOk))
            .catch((err) => send(res, 500, err));
        }
    });
    
    router.get('/feed/last', (req, res) => {
        cat.getLastFeed()
            .then(data => res.send(data))
            .catch(err => send(res, 500, err));
    });
    router.get('/feed', (req, res) => {
        
        res.send(cat.getFeeds());
            //.then(data => res.send(data))
            //.catch(err => send(res, 500, err));
    });
    
    router.get('/schedules', (req, res) => {
        res.send(cat.getSchedules());
    })
    
    router.post('/schedules', (req, res) => {
       if (!req.body || req.body.constructor !== Array) {
           res.sendStatus(400);
       } else {
           for (var i = 0; i < req.body.length; i++) {
               if( ! ('hour' in req.body[i])
                 || ! ('minute' in req.body[i])
                 || ! ('quantity' in req.body[i])
               ) {
                   res.sendStatus(400);
               }
           }
           cat.setSchedules(req.body)
            .then(data => res.send(data))
            .catch(err => send(res, 500, err));
       } 
    });
    
    router.get('/schedules/next', (req,res) => {
       res.send(cat.getNextSchedule()); 
    });
    
    
    
    app.use('/api', router);
    
    // START THE SERVER
    // =============================================================================
    app.listen(port);
    console.log('Cat Feeder is listening on port ' + port);
    
    
    


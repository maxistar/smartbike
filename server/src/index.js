import http from 'http';
import express from 'express';
import cors from 'cors';
import morgan from 'morgan';
import bodyParser from 'body-parser';
import initializeDb from './db';
import middleware from './middleware';
import api0 from './api/v0';
import api1 from './api/v1';
import config from './config.json';
import connection from 'mysql';
import dotenv from 'dotenv';
import md5File from 'md5-file'
dotenv.config();

let app = express();
app.server = http.createServer(app);

// logger
app.use(morgan('dev'));

// 3rd party middleware
app.use(cors({
    exposedHeaders: config.corsHeaders
}));

app.use(bodyParser.json({
    limit : config.bodyLimit
}));

// connect to db
initializeDb(connection,(db) => {

    // internal middleware
    app.use(middleware({ config, db }));

    // static
    app.use(express.static(process.env.WEB_ROOT,
        {
            'setHeaders': setHeaders
        }
    ));

    // Set header to force download
    function setHeaders (res, path) {
        console.log(path);
        res.setHeader('MD5', md5File.sync(path))
    }

    // api router for api v0
    app.use('/api', api0({ config, db }));

    // api router for api v1
    app.use('/api/1', api1({ config, db }));

    app.server.listen(process.env.PORT || config.port, () => {
        console.log(`Started on port ${app.server.address().port}`);
    });
});

export default app;

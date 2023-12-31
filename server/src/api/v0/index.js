import { version } from '../../../package.json';
import { Router } from 'express';

export default ({ config, db }) => {
    let api = Router();

    // perhaps expose some API metadata at the root
    api.get('/', (req, res) => {
        res.json({ version });
    });

    api.get('/firmware/', (req, res) => {
        db.query(
            "select id, version, size, md5, marketing_version from firmware ORDER BY id DESC LIMIT 1",
            (err, rows) => {
                res.json(rows[0]);
            });
    });

    api.get('/firmware/latest', (req, res) => {
        db.query(
            "SELECT id, version, size, md5, marketing_version FROM firmware ORDER BY id DESC LIMIT 1",
            (err, rows) => {
                res.json(rows[0]['id']);
            });
    });

    api.get('/info/latest/', (req, res) => {
        db.query(
            'SELECT * FROM `status` '
            + 'ORDER BY `id` DESC LIMIT 1',
            (err, rows) => {
                if (err) {
                    console.log(err);
                    return res.json({ status:"error"});
                }
                res.json(rows[0]);
            }
        )
    });

    api.post('/status/', (req, res) => {
        console.log(req.body);
        db.query(
            'INSERT INTO `status` '
                + '(`timestamp`, `device_id`, `version`, `power_source`, '
                + '`battery_status`, `battery`, `battery_info`) '
                + ' VALUES (now(), ?, ?, ?, ?, ?, ?)',
            [
                req.body.deviceID,
                req.body.version,
                req.body.powerSource,
                req.body.batteryStatus,
                req.body.battery,
                JSON.stringify(req.body.batteryInfo)
            ],
            (err, result) => {
                if (err) {
                    console.log(err);
                    return res.json({ status:"error"});
                }
                res.json({ status:"success", result:result });
            }
        )
    });

    return api;
}

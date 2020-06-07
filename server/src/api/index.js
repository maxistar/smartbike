import { version } from '../../package.json';
import { Router } from 'express';
import facets from './facets';

export default ({ config, db }) => {
	let api = Router();

	// mount the facets resource
	api.use('/facets', facets({ config, db }));

	// perhaps expose some API metadata at the root
	api.get('/', (req, res) => {
		res.json({ version });
	});

	api.get('/status/', (req, res) => {
		res.json({ version });
	});

	api.post('/status/', (req, res) => {
		console.log(req.body);
		db.query(
			'INSERT INTO `status` (`timestamp`, power_source, battery_status, battery) VALUES (now(), ?, ?, ?)',
			[req.body.powerSource, req.body.batteryStatus, req.body.battery],
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

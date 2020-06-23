import { Router } from 'express';

export default ({ config, db }) => {
	let routes = Router();

    console.log('middleware!!!');
	// add middleware here

	return routes;
}

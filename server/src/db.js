export default (db, callback) => {
	// connect to a database if needed, then pass it to `callback`:
	const connection = db.createPool({
		host: process.env.DB_HOST,
		user: process.env.DB_USER,
		password: process.env.DB_PASS,
		database: process.env.DB_NAME
	})
	callback(connection);
}

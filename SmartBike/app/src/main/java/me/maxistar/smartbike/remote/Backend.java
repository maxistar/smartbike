package me.maxistar.smartbike.remote;

import android.os.AsyncTask;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import me.maxistar.smartbike.BuildConfig;
import me.maxistar.smartbike.ui.home.HomeViewModel;

public class Backend {

    HomeViewModel model;

    public Backend(HomeViewModel model) {
        this.model = model;
    }

    public void requestServer() {
        DataModel data = new DataModel();
        model.setText(data);
        new RetrieveStatusTask().execute(BuildConfig.API_URL + "/api/info/latest");
    }

    class RetrieveStatusTask extends AsyncTask<String, Void, DataModel> {

        protected DataModel doInBackground(String... urls) {
            DataModel data = new DataModel();
            SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
            dateFormat.setTimeZone(TimeZone.getTimeZone("UTC"));
            try {
                String serverResponse = sendGET(urls[0]);
                JSONObject result = new JSONObject(serverResponse);
                data.dateTime = dateFormat.parse(result.getString("timestamp"));
                String jsonData = result.getString("battery_info");
                JSONObject batteryInfo = new JSONObject(jsonData);
                data.longitude = batteryInfo.getDouble("longitude");
                data.latitude = batteryInfo.getDouble("latitude");
                data.batteryValue = batteryInfo.getDouble("battery");
                data.solarBattery = batteryInfo.getDouble("solarBattery");
                data.sleepTime = batteryInfo.getInt("sleep");
                data.executionTime = batteryInfo.getInt("execution");
                data.version = result.getInt("version");
                return data;
            } catch (JSONException | IOException | ParseException e) {
                e.printStackTrace();
                data.serverResponse = e.toString();
                return data;
            }
        }

        /**
         * Returns data from server
         *
         * @see "https://www.journaldev.com/7148/java-httpurlconnection-example-java-http-request-get-post"
         * @param url Url
         * @return Returns value
         * @throws IOException If network error
         */
        private String sendGET(String url) throws IOException {
            URL obj = new URL(url);
            HttpURLConnection con = (HttpURLConnection) obj.openConnection();
            con.setRequestMethod("GET");
            con.setRequestProperty("User-Agent", "android-app");
            con.setReadTimeout(10000);
            con.setConnectTimeout(15000);
            StringBuffer response = new StringBuffer();
            int responseCode = con.getResponseCode();
            System.out.println("GET Response Code :: " + responseCode);
            if (responseCode == HttpURLConnection.HTTP_OK) { // success
                BufferedReader in = new BufferedReader(new InputStreamReader(
                        con.getInputStream()));
                String inputLine;

                while ((inputLine = in.readLine()) != null) {
                    response.append(inputLine);
                }
                in.close();

                System.out.println(response);
            } else {
                System.out.println("GET request not worked");
            }
            return response.toString();
        }

        protected void onPostExecute(DataModel result) {
            model.setText(result);
        }
    }

}

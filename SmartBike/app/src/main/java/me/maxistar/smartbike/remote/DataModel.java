package me.maxistar.smartbike.remote;

import java.util.Date;

public class DataModel {

    public String serverResponse = "";

    public String dateTime = "";

    double latitude = 0;

    double longitude = 0;

    double batteryValue = 0;

    double solarBattery = 0;

    boolean loaded = false;

    @Override
    public String toString() {
        return "DataModel{" +
                "serverResponse='" + serverResponse + '\'' +
                ", dateTime='" + dateTime + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                ", batteryValue=" + batteryValue +
                ", solarBattery=" + solarBattery +
                ", loaded=" + loaded +
                '}';
    }
}

package me.maxistar.smartbike.remote;

import androidx.annotation.NonNull;

import java.util.Date;

public class DataModel {

    public String serverResponse = "";

    public Date dateTime;

    public double latitude = 0;

    public double longitude = 0;

    public double batteryValue = 0;

    public double solarBattery = 0;

    boolean loaded = false;

    @NonNull
    @Override
    public String toString() {
        return "DataModel{" +
                "serverResponse='" + serverResponse + '\'' +
                ", dateTime='" + dateTime.toString() + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                ", batteryValue=" + batteryValue +
                ", solarBattery=" + solarBattery +
                ", loaded=" + loaded +
                '}';
    }
}

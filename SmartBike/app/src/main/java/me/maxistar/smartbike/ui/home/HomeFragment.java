package me.maxistar.smartbike.ui.home;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import java.text.SimpleDateFormat;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.Locale;
import java.util.Objects;
import java.util.TimeZone;

import me.maxistar.smartbike.R;
import me.maxistar.smartbike.databinding.FragmentHomeBinding;
import me.maxistar.smartbike.remote.Backend;
import me.maxistar.smartbike.remote.DataModel;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        HomeViewModel homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView timeValue = binding.timeValue;
        final TextView latitudeValue = binding.latitudeValue;
        final TextView longitudeValue = binding.longitudeValue;
        final TextView batteryValue = binding.batteryValue;
        final TextView soloarBatteryValue = binding.solarBatteryValue;
        final TextView versionValue = binding.versionValue;
        final TextView nextCallValue = binding.nextCallValue;
        final TextView executionTimeValue = binding.executionTimeValue;


        homeViewModel.getText().observe(
                getViewLifecycleOwner(),
                dataModel -> {
                    if (dataModel.dateTime != null) {
                        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
                        dateFormat.setTimeZone(TimeZone.getTimeZone("Europe/Berlin"));
                        String formattedDate = dateFormat.format(dataModel.dateTime);
                        timeValue.setText(formattedDate);


                        Date value = new Date(dataModel.dateTime.getTime() + dataModel.sleepTime * 1000L);
                        nextCallValue.setText(dateFormat.format(value));

                    }
                    executionTimeValue.setText(requireContext().getString(R.string.ms, dataModel.executionTime));
                    latitudeValue.setText(String.valueOf(dataModel.latitude));
                    longitudeValue.setText(String.valueOf(dataModel.longitude));
                    batteryValue.setText(String.valueOf(dataModel.batteryValue));
                    soloarBatteryValue.setText(String.valueOf(dataModel.solarBattery));
                    versionValue.setText(String.valueOf(dataModel.version));
                }
        );

        Backend backend = new Backend(homeViewModel);
        backend.requestServer();

        return root;
    }

    public static void setText(DataModel data) {

    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}
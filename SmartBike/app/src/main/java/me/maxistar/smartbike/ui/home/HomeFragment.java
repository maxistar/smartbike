package me.maxistar.smartbike.ui.home;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
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

        final TextView textView = binding.textHome;
        homeViewModel.getText().observe(
                getViewLifecycleOwner(),
                users -> {
                    textView.setText(users.toString());
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
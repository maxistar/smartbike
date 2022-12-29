package me.maxistar.smartbike.ui.home;

import java.util.Date;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;
import me.maxistar.smartbike.remote.DataModel;

public class HomeViewModel extends ViewModel {

    private final MutableLiveData<DataModel> mText;

    // DataModel data = null;
    public HomeViewModel() {
        mText = new MutableLiveData<>();
        DataModel data = new DataModel();
        mText.setValue(data);
    }

    public void setText(DataModel value) {
        mText.setValue(value);
    }

    public LiveData<DataModel> getText() {
        return mText;
    }

}
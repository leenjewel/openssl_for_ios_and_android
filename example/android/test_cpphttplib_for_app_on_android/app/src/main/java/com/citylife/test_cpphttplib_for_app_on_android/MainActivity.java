package com.citylife.test_cpphttplib_for_app_on_android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.InputStream;
import java.util.Vector;
import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "demo";
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static Vector<String> getFileName(String fileAbsolutePath) {
        Vector<String> vecFile = new Vector<String>();
        File file = new File(fileAbsolutePath);
        File[] subFile = file.listFiles();

        for (int iFileLength = 0; iFileLength < subFile.length; iFileLength++) {
            // 判断是否为文件夹
            if (!subFile[iFileLength].isDirectory()) {
                String filename = subFile[iFileLength].getName();
                Log.e("eee","文件名 ： " + filename);
            }
        }

        return vecFile;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String resourceDir = getApplicationContext().getPackageResourcePath();
        Log.i(TAG, resourceDir);
        File file = new File(resourceDir);
        boolean isDir = file.isDirectory();
        Log.i(TAG, String.valueOf(isDir));
        if (isDir) {
            this.getFileName(resourceDir);
        }

        try {
            InputStream in = getResources().openRawResource(R.raw.ca_bundle);
            int length = in.available();
            if (0 == length) {
                return;
            }
            byte[] buffer = new byte[length];
            in.read(buffer);
        } catch (Exception e) {
            e.printStackTrace();
        }



        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}

package com.citylife.test_cpphttplib_for_app_on_android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.os.Environment;

import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Vector;
import java.io.File;
import java.lang.String;

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



            /**
             * ref: https://blog.csdn.net/hacker_Lees/article/details/78593799
             */
            String tmpDir = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test_cpphttplib_for_app_on_android/tmp";
            String cerFileName = tmpDir + "/ca_bundle.crt";
            File outFile = new File(cerFileName);
            if (!outFile.getParentFile().exists()) {
                createDir(tmpDir);
            }
            if (!outFile.exists()) {
                boolean ret = outFile.createNewFile();
                Log.i(TAG, String.valueOf(ret));
                InputStream in = getResources().openRawResource(R.raw.ca_bundle);
                int length = in.available();
                if (0 == length) {
                    Log.i(TAG, String.valueOf(length));
                    return;
                }
                byte[] buffer = new byte[length];
                in.read(buffer);

                FileOutputStream outputStream = new FileOutputStream(outFile);
                outputStream.write(buffer);
                Log.i(TAG, cerFileName);
            }


        } catch (Exception e) {
            e.printStackTrace();
        }



        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    public static String createDir(String dirPath){
        //因为文件夹可能有多层，比如:  a/b/c/ff.txt  需要先创建a文件夹，然后b文件夹然后...
        try{
            File file=new File(dirPath);
            if(file.getParentFile().exists()){
                Log.i(TAG, "----- 创建文件夹" + file.getAbsolutePath());
                file.mkdir();
                return file.getAbsolutePath();
            }
            else {
                createDir(file.getParentFile().getAbsolutePath());
                Log.i(TAG,"----- 创建文件夹" + file.getAbsolutePath());
                file.mkdir();
            }

        }catch (Exception e){
            e.printStackTrace();
        }
        return dirPath;
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}

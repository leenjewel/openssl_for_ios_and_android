package com.github.leenjewel.openssl_and_curl;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final TextView tv = (TextView) findViewById(R.id.sample_text);
        final EditText et = (EditText) findViewById(R.id.md5_input);
        final Button btn = (Button) findViewById(R.id.md5_button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String input = et.getText().toString();
                if (TextUtils.isEmpty(input)) {
                    tv.setText("Input Please");
                } else {
                    String output = stringFromMD5(input);
                    // String output = stringFromJNI();
                    tv.setText("MD5("+input+") = "+output);
                }
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String stringFromMD5(String input);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}

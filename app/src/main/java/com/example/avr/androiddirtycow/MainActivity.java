package com.example.avr.androiddirtycow;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        // Get Button to Add Click Event
        Button button;
        button = (Button)findViewById(R.id.buttonGo);

        // Add Click Event
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                // Get Values from TextBoxes
                EditText editTextFile = findViewById(R.id.editTextFilePath);
                EditText editTextOffset = findViewById(R.id.editTextOffset);
                EditText editTextReplace = findViewById(R.id.editTextReplacementText);

                String filePath = editTextFile.getText().toString();
                int offset = Integer.parseInt(editTextOffset.getText().toString());
                String replaceText = editTextReplace.getText().toString();


                // Create & Display Toast Message
                Context context = getApplicationContext();
                String message = String.format(Locale.getDefault(),
                        "DirtyCOW: %s, %d, %s", filePath, offset, replaceText);

                Toast toastMessage = Toast.makeText(context, message, Toast.LENGTH_LONG);
                toastMessage.show();


                // Call JNI DirtyCOW function
                dirtyCOW_JNI(filePath, offset, replaceText);
            }
        });

        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native boolean dirtyCOW_JNI(String path, int offset, String replacementText);
}

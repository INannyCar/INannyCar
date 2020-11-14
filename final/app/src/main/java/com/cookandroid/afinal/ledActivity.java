package com.cookandroid.afinal;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class ledActivity extends AppCompatActivity {
    private Button btn_LED,btn_zi;
    private boolean status=false;
    private boolean statuszi=false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_led);
        btn_LED = (Button) findViewById(R.id.button1);
        btn_LED.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!status){
                    btn_LED.setText("LED ON");
                    status=true;
                }else{
                    btn_LED.setText("LED OFF");
                    status=false;
                }
            }
        });
        btn_zi = (Button) findViewById(R.id.button2);
        btn_zi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!statuszi){
                    btn_zi.setText("자동모드 ON");
                    statuszi=true;
                }else{
                    btn_zi.setText("자동모드 OFF");
                    statuszi=false;
                }
            }
        });
    }
}

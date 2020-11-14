package com.cookandroid.afinal;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.TextView;

public class wenduActivity extends AppCompatActivity {

    private TextView show;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wendu);

        show = findViewById(R.id.textView1);
    }
    public void click(View View){
        show.setText("측정 중");
        blink();
    }
private void blink(){
    final Handler handler = new Handler();
    new Thread(new Runnable() {
        @Override
        public void run() {
            int timeToBlink = 1000;    //in milissegunds
            try{Thread.sleep(timeToBlink);}catch (Exception ignored) {}
            handler.post(new Runnable() {
                @Override
                public void run() {
                    TextView txt = (TextView) findViewById(R.id.textView1);
                    if(txt.getVisibility() == View.VISIBLE){
                        txt.setVisibility(View.INVISIBLE);
                    }else{
                        txt.setVisibility(View.VISIBLE);
                    }
                    blink();
                }
            });
        }
    }).start();
}
}

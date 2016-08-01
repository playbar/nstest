package com.jqbar;

import java.io.File;
import java.io.FilenameFilter;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.jqbar.R;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ListActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MenuItem.OnMenuItemClickListener;
import android.view.View;
import android.view.View.OnCreateContextMenuListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

public class LocalFlashAct extends ListActivity
{

    private static final int MENU_HOME = Menu.FIRST;

    private static final int MENU_FILE_PLAY = Menu.FIRST + 2;
    private static final int MENU_FILE_DELETE = Menu.FIRST + 3;

	private String LOCAL_PATH = null;
    Intent mIntent = null;
	Bundle mBundle = null;
	ListView mlistView = null;
	private Button localBackBtn;

	class Info
	{
		String title;
		String summary;
		Bitmap icon;
	}
	private static List<Info> mFlashFileList = new ArrayList<Info>();

    
    public boolean onCreateOptionsMenu(Menu menu)
    {
        super.onCreateOptionsMenu(menu);
        //menu.add(0, MENU_HOME, 0, "HOME");
        return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
            case MENU_HOME:
            	//gv.gThread.setRunning(false);
            	Intent intent = new Intent(this, AboutActivity.class);
            	startActivity(intent);
            	LocalFlashAct.this.finish();
                return true;
        }

        return false;
    }

    
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);
    	mFlashFileList.clear();

        requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		GlobalFun.setOrientation( this );
		
    	setContentView(R.layout.localflashact );
    	mIntent = this.getIntent();
    	mBundle = mIntent.getExtras();
    	LOCAL_PATH = mBundle.getString("LOCAL_PATH");
    	GetFlashList();
    	setListListener();
    	localBackBtn=(Button)findViewById(R.id.local_back_btn);
    	localBackBtn.setOnClickListener(new Button.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				LocalFlashAct.this.finish();
			}
		});

    }

    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
    	switch (keyCode)
		{
		case KeyEvent.KEYCODE_BACK:
			return true;
		default:
			return super.onKeyDown( keyCode, event);
		}
    }

    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
    	switch (keyCode)
		{
		case KeyEvent.KEYCODE_BACK:
			finish();
			return true;
		default:
			return super.onKeyUp(keyCode, event);
		}
    }

    void setListListener()
    {
    	mlistView = getListView();
    	mlistView.setOnItemLongClickListener(new OnItemLongClickListener()
    	{
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id)
            {
            	selectedItem = position;
                return false;

            }
        });
    	mlistView.setOnCreateContextMenuListener(new OnCreateContextMenuListener()
    	{

            public void onCreateContextMenu(ContextMenu menu, View view,ContextMenuInfo arg2)
            {
                menu.setHeaderTitle( R.string.file_option );
                menu.setHeaderIcon(R.drawable.logo_36);
                menu.add(0, MENU_FILE_PLAY, 0, R.string.file_paly ).setOnMenuItemClickListener(mOnMenuItemClickListener);
                menu.add(0, MENU_FILE_DELETE, 1, R.string.file_delete ).setOnMenuItemClickListener(mOnMenuItemClickListener);
            }
        });
    	mlistView.setOnItemClickListener(new OnItemClickListener()
    	{

            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
            	selectedItem = position;
        		Info info = mFlashFileList.get( position );
        		String strFile = LOCAL_PATH + "/" +  info.title + ".swf";

        		if( strFile.length() > 0 )
        		{
        			mBundle.clear();
        			mBundle.putString("strFile", strFile);
        			mIntent.putExtras( mBundle );
        			setResult(RESULT_OK, mIntent );
        			finish();
        		}
        		return;
            }
        });
     }
    int selectedItem;
    final OnMenuItemClickListener mOnMenuItemClickListener = new OnMenuItemClickListener()
    {

        public boolean onMenuItemClick(MenuItem item)
        {
            switch (item.getItemId())
            {
            case MENU_FILE_PLAY:  // play
	            {
	        		Info info = mFlashFileList.get( selectedItem );
	        		String strFile = LOCAL_PATH + "/" +  info.title + ".swf";

	        		if( strFile.length() > 0 )
	        		{
	        			mBundle.clear();
	        			mBundle.putString("strFile", strFile);
	        			mIntent.putExtras( mBundle );
	        			setResult(RESULT_OK, mIntent );
	        			finish();
	        		}
	            }
                break;
            case MENU_FILE_DELETE:  // delete
	            {
	                Info info = mFlashFileList.get( selectedItem );
	                final String strFile = LOCAL_PATH + "/" +  info.title + ".swf";
	                Dialog dlg = new AlertDialog.Builder(LocalFlashAct.this)
	                .setIcon(R.drawable.logo_36)
	                .setTitle("是否删除 " + info.title + " ?" )
	                .setPositiveButton("确定", new DialogInterface.OnClickListener()
	                {
	                    public void onClick(DialogInterface dialog, int whichButton)
	                    {
	                        File file = new File(strFile);
	                        if (file.exists())
	                        {
	                            file.delete();
	                            mFlashFileList.remove(selectedItem);
	                            EfficientAdapter sAdapter = (EfficientAdapter)mlistView.getAdapter();
	                            sAdapter.notifyDataSetChanged();
	                        }
	                    }
	                }).setNegativeButton("取消", new DialogInterface.OnClickListener(){
	                    public void onClick(DialogInterface dialog, int which) {

	                    }
	                })
	                .create();
	                dlg.show();
	                /////////////////
	            }
                break;
            }
            return false;
        }
    };


    public void GetFlashList()
	{

		if( LOCAL_PATH == null )
		{
			return;
		}
		File home = new File(LOCAL_PATH);
		if( ! home.exists() )
		    return;
		if (home.listFiles(new FlashFilter()).length > 0)
		{
			for (File file : home.listFiles(new FlashFilter()))
			{
				Info info = new Info();
				String tmp = file.getName();
				tmp = tmp.substring(0, tmp.length() - 4);
				info.title = tmp;
				float len = file.length();
				len = len / (1024*1024);

				Date date = new Date(file.lastModified());

				SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
				DecimalFormat df = new DecimalFormat("0.00");

				info.summary = "[ " + sdf.format(date) + "    " +  df.format(len) + " MB ]";

				mFlashFileList.add(info);
			}

		}
        setListAdapter(new EfficientAdapter(this));
	}

	class FlashFilter implements FilenameFilter
	{
		public boolean accept(File dir, String name)
		{
			return (name.endsWith(".swf"));
		}
	}// end class of FlashFilter

    private static class EfficientAdapter extends BaseAdapter
    {
        private LayoutInflater mInflater;
        private Bitmap mIcon1;

        public EfficientAdapter(Context context) {
            // Cache the LayoutInflate to avoid asking for a new one each time.
            mInflater = LayoutInflater.from(context);

            // Icons bound to the rows.
            mIcon1 = BitmapFactory.decodeResource(context.getResources(), R.drawable.logo_36);
        }


        public int getCount()
        {
            return mFlashFileList.size();

        }

        /**
         * Since the data comes from an array, just returning the index is
         * sufficent to get at the data. If we were using a more complex data
         * structure, we would return whatever object represents one row in the
         * list.
         *
         * @see android.widget.ListAdapter#getItem(int)
         */
        public Object getItem(int position)
        {
            return position;
        }

        /**
         * Use the array index as a unique id.
         *
         * @see android.widget.ListAdapter#getItemId(int)
         */
        public long getItemId(int position) {
            return position;
        }

        /**
         * Make a view to hold each row.
         *
         * @see android.widget.ListAdapter#getView(int, android.view.View,
         *      android.view.ViewGroup)
         */
        public View getView(int position, View convertView, ViewGroup parent) {
            // A ViewHolder keeps references to children views to avoid unneccessary calls
            // to findViewById() on each row.
            ViewHolder holder;

            // When convertView is not null, we can reuse it directly, there is no need
            // to reinflate it. We only inflate a new View when the convertView supplied
            // by ListView is null.
            if (convertView == null) {
                convertView = mInflater.inflate(R.layout.list_item_icon_text, null);

                // Creates a ViewHolder and store references to the two children views
                // we want to bind data to.
                holder = new ViewHolder();
                holder.title = (TextView) convertView.findViewById(R.id.title);
                holder.summary = (TextView) convertView.findViewById(R.id.summary);
                holder.icon = (ImageView) convertView.findViewById(R.id.icon);

                convertView.setTag(holder);
            } else {
                // Get the ViewHolder back to get fast access to the TextView
                // and the ImageView.
                holder = (ViewHolder) convertView.getTag();
            }

            // Bind the data efficiently with the holder.
            Info info = mFlashFileList.get( position );
            holder.title.setText(info.title);
            holder.summary.setText(info.summary);
            holder.icon.setImageBitmap(mIcon1);

            return convertView;
        }

        static class ViewHolder {
            TextView title;
            TextView summary;
            ImageView icon;
        }
    }

}

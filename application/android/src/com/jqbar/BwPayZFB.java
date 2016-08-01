package com.jqbar;

import java.net.URLEncoder;
import java.security.KeyFactory;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import com.alipay.android.app.sdk.AliPay;
import com.unionpay.uppay.PayActivity;

import android.R.integer;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;


public class BwPayZFB  {
	private Activity mActivity;
	public int chargeView_width,chargeView_height;
	public static final String SIGN_ALGORITHMS = "SHA1WithRSA";
	static private final int BASELENGTH = 128;
	static private final int LOOKUPLENGTH = 64;
	static private final int TWENTYFOURBITGROUP = 24;
	static private final int EIGHTBIT = 8;
	static private final int SIXTEENBIT = 16;
	static private final int FOURBYTE = 4;
	static private final int SIGN = -128;
	static private final char PAD = '=';
	static private final boolean fDebug = false;
	static final private byte[] base64Alphabet = new byte[BASELENGTH];
	static final private char[] lookUpBase64Alphabet = new char[LOOKUPLENGTH];

	static {
		for (int i = 0; i < BASELENGTH; ++i) {
			base64Alphabet[i] = -1;
		}
		for (int i = 'Z'; i >= 'A'; i--) {
			base64Alphabet[i] = (byte) (i - 'A');
		}
		for (int i = 'z'; i >= 'a'; i--) {
			base64Alphabet[i] = (byte) (i - 'a' + 26);
		}

		for (int i = '9'; i >= '0'; i--) {
			base64Alphabet[i] = (byte) (i - '0' + 52);
		}

		base64Alphabet['+'] = 62;
		base64Alphabet['/'] = 63;

		for (int i = 0; i <= 25; i++) {
			lookUpBase64Alphabet[i] = (char) ('A' + i);
		}

		for (int i = 26, j = 0; i <= 51; i++, j++) {
			lookUpBase64Alphabet[i] = (char) ('a' + j);
		}

		for (int i = 52, j = 0; i <= 61; i++, j++) {
			lookUpBase64Alphabet[i] = (char) ('0' + j);
		}
		lookUpBase64Alphabet[62] = (char) '+';
		lookUpBase64Alphabet[63] = (char) '/';

	}
	private static final Map<String, String> sResultStatus;
	static {
		sResultStatus = new HashMap<String, String>();
		sResultStatus.put("9000", "操作成功");
		sResultStatus.put("4000", "系统异常");
		sResultStatus.put("4001", "数据格式不正确");
		sResultStatus.put("4003", "该用户绑定的支付宝账户被冻结或不允许支付");
		sResultStatus.put("4004", "该用户已解除绑定");
		sResultStatus.put("4005", "绑定失败或没有绑定");
		sResultStatus.put("4006", "订单支付失败");
		sResultStatus.put("4010", "重新绑定账户");
		sResultStatus.put("6000", "支付服务正在进行升级操作");
		sResultStatus.put("6001", "用户中途取消支付操作");
		sResultStatus.put("7001", "网页支付失败");
	}

	//商户私钥，自助生成
	public static final String PRIVATE = 
"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBANHxZvq+ay3nXEsYY7YI7OkgzzR0HAN4/2Hh77xHkD+VDm7RSUAyDl7Vq0uElbchvswncVmWwwPRX9ZMd9NGtYxlIVec3VnxufJZr7GS9WQcvWXkWsNUxh/d9yRizYh/9MB/0+6E+aO8dPPwFvETg6cJlA5VjiH6WqCGz8Px9ER3AgMBAAECgYEAkhhxVzBHmYwU99UjHjwb0SeZ"
+"yaC7FdePsGfv0o/iW7y4bHmLhfbPdUMvjr/XB7ed9KfGA1gcoX2EObbA8++SybtRArTpY2roSJFPa1GQy6DjjijpHfgOlI3fv3mPdV65RmlBNvnLSWFGPs+slFK9iREA/09PjNWN/HHojw+dsQECQQDzlEiwohyGysxkJhsK1x0rY9jbK2RG1z/2RJRsQdSrmfzYOqBuof8nUTlz3E9gOuqfcbw0BrBKrUPDVd/5kXxBAkEA3KYGrtJH2HZl4rjF"
+"FTaksqSEJyzion3UpGozC2CFDvnn9WD3/CB1JaZ+zh6FMEgkXqveBhkB3PjOGLyJj7/ytwJAbA/mK87P3xELmoa5SpbizwQuChX3F+KY8tW8D0cpZbQcU4W29T4TZ8sGNRuRb3zT6EbaGy64eXji7j8j7aGYwQJADS8ufwu5l0mb1OxNenMFbFdAPkcEU8lJB3oBCtD9omN0ZMkiiEI4PcOxGAfi1Xu6EJ4caijcX6POTNQXNCu4CQJARvX9K8Ye"
+"I0H0KY50foUBN433ZR1lYZqMKcR2tIZu5EadQyVb/Qz8glYQNv+CXgK6KFqhYyAnwp+dE5U0+AR/iQ==";
	public static final String PUBLIC="";
	private static final int RQF_PAY = 1;
	private static final int RQF_LOGIN = 2;

	public BwPayZFB(Activity c,int width,int height) {
		mActivity=c;
		this.chargeView_width=width;
		this.chargeView_height=height;
	}
	public void onTouchCpp() {
		((MobileMain)mActivity).mMobileView.onTouchUpCpp(50, 50, 0);
	}
	public int getChargeView_width() {
		return chargeView_width;
	}

	public void setChargeView_width(int chargeView_width) {
		this.chargeView_width = chargeView_width;
	}

	public int getChargeView_height() {
		return chargeView_height;
	}

	public void setChargeView_height(int chargeView_height) {
		this.chargeView_height = chargeView_height;
	}

	private static boolean isWhiteSpace(char octect) {
		return (octect == 0x20 || octect == 0xd || octect == 0xa || octect == 0x9);
	}

	private static boolean isPad(char octect) {
		return (octect == PAD);
	}

	private static boolean isData(char octect) {
		return (octect < BASELENGTH && base64Alphabet[octect] != -1);
	}
	
	private static int removeWhiteSpace(char[] data) {
		if (data == null) {
			return 0;
		}

		// count characters that's not whitespace
		int newSize = 0;
		int len = data.length;
		for (int i = 0; i < len; i++) {
			if (!isWhiteSpace(data[i])) {
				data[newSize++] = data[i];
			}
		}
		return newSize;
	}
	
	private String getSignType() {
		return "sign_type=\"RSA\"";
	}
	
	private String getNewOrderInfo(String out_trade_no,String subject,String totol_free,String notify_url,String partner,String seller_id) {
		StringBuilder sb = new StringBuilder();
		sb.append("partner=\"");
		sb.append(partner);
		sb.append("\"&out_trade_no=\"");
		sb.append(out_trade_no);
		sb.append("\"&subject=\"");
		sb.append(subject);
		sb.append("\"&body=\"");
		sb.append(subject);
		sb.append("\"&total_fee=\"");
		sb.append(totol_free);
		sb.append("\"&notify_url=\"");

		// 网址需要做URL编码
		sb.append(URLEncoder.encode(notify_url));
		sb.append("\"&service=\"mobile.securitypay.pay");
		sb.append("\"&_input_charset=\"UTF-8");
		sb.append("\"&return_url=\"");
		sb.append(URLEncoder.encode("http://m.alipay.com"));
		sb.append("\"&payment_type=\"1");
		sb.append("\"&seller_id=\"");
		sb.append("hmarket@jqbar.com");

		// 如果show_url值为空，可不传
		// sb.append("\"&show_url=\"");
		sb.append("\"&it_b_pay=\"30m");
		sb.append("\"");

		return new String(sb);
	}
	
	Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			Result result = new Result((String) msg.obj);

			switch (msg.what) {
			case RQF_PAY:
			case RQF_LOGIN: {
				Toast.makeText(mActivity, result.getResult(),
						Toast.LENGTH_SHORT).show();

			}
				break;
			default:
				break;
			}
		};
	};
	
	public static boolean doCheck(String content, String sign, String publicKey) {
		try {
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
			byte[] encodedKey = decode(publicKey);
			PublicKey pubKey = keyFactory
					.generatePublic(new X509EncodedKeySpec(encodedKey));

			java.security.Signature signature = java.security.Signature
					.getInstance(SIGN_ALGORITHMS);

			signature.initVerify(pubKey);
			signature.update(content.getBytes("utf-8"));
			Log.i("Result", "content :   "+content);
			boolean bverify = signature.verify(decode(sign));
			Log.i("Result","bverify = " + bverify);
			return bverify;
		} catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	public int pay(String strUrl) {
	if (mActivity == null) {
		return -1;
	}	
	try{
	String wares[] = strUrl.split("\\|");
	String out_trade_no = wares[1];
	String subject = wares[2];
	String totol_free = wares[3];
	String notify_url =wares[4];
	String partner =wares[5];
	String seller_id =wares[6];
	String info = getNewOrderInfo(out_trade_no,subject,totol_free,notify_url,partner,seller_id);
	String sign = sign(info, PRIVATE);
	sign = URLEncoder.encode(sign);
	info += "&sign=\"" + sign + "\"&" + getSignType();
	final String orderInfo = info;
	new Thread() {
		public void run() {
			AliPay alipay = new AliPay(mActivity, mHandler);
			
			//设置为沙箱模式，不设置默认为线上环境
			//alipay.setSandBox(true);
			String result = alipay.pay(orderInfo);
			Log.i("resultaa", "result = " + result);
			Message msg = new Message();
			msg.what = RQF_PAY;
			msg.obj = result;
			mHandler.sendMessage(msg);
		}
	}.start();

} catch (Exception ex) {
	ex.printStackTrace();
	Toast.makeText(mActivity, "Failure calling remote service",
			Toast.LENGTH_SHORT).show();
}
	return 0;
	}

	public static String sign(String content, String privateKey) {
		String charset = "UTF-8";
		try {
			PKCS8EncodedKeySpec priPKCS8 = new PKCS8EncodedKeySpec(
					decode(privateKey));
			KeyFactory keyf = KeyFactory.getInstance("RSA");
			PrivateKey priKey = keyf.generatePrivate(priPKCS8);

			java.security.Signature signature = java.security.Signature
					.getInstance(SIGN_ALGORITHMS);

			signature.initSign(priKey);
			signature.update(content.getBytes(charset));

			byte[] signed = signature.sign();

			return encode(signed);
		} catch (Exception e) {
			e.printStackTrace();
		}

		return null;
	}
	public static String encode(byte[] binaryData) {

		if (binaryData == null) {
			return null;
		}

		int lengthDataBits = binaryData.length * EIGHTBIT;
		if (lengthDataBits == 0) {
			return "";
		}

		int fewerThan24bits = lengthDataBits % TWENTYFOURBITGROUP;
		int numberTriplets = lengthDataBits / TWENTYFOURBITGROUP;
		int numberQuartet = fewerThan24bits != 0 ? numberTriplets + 1
				: numberTriplets;
		char encodedData[] = null;

		encodedData = new char[numberQuartet * 4];

		byte k = 0, l = 0, b1 = 0, b2 = 0, b3 = 0;

		int encodedIndex = 0;
		int dataIndex = 0;
		if (fDebug) {
			System.out.println("number of triplets = " + numberTriplets);
		}

		for (int i = 0; i < numberTriplets; i++) {
			b1 = binaryData[dataIndex++];
			b2 = binaryData[dataIndex++];
			b3 = binaryData[dataIndex++];

			if (fDebug) {
				System.out.println("b1= " + b1 + ", b2= " + b2 + ", b3= " + b3);
			}

			l = (byte) (b2 & 0x0f);
			k = (byte) (b1 & 0x03);

			byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2)
					: (byte) ((b1) >> 2 ^ 0xc0);
			byte val2 = ((b2 & SIGN) == 0) ? (byte) (b2 >> 4)
					: (byte) ((b2) >> 4 ^ 0xf0);
			byte val3 = ((b3 & SIGN) == 0) ? (byte) (b3 >> 6)
					: (byte) ((b3) >> 6 ^ 0xfc);

			if (fDebug) {
				System.out.println("val2 = " + val2);
				System.out.println("k4   = " + (k << 4));
				System.out.println("vak  = " + (val2 | (k << 4)));
			}

			encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[val2 | (k << 4)];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[(l << 2) | val3];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[b3 & 0x3f];
		}

		// form integral number of 6-bit groups
		if (fewerThan24bits == EIGHTBIT) {
			b1 = binaryData[dataIndex];
			k = (byte) (b1 & 0x03);
			if (fDebug) {
				System.out.println("b1=" + b1);
				System.out.println("b1<<2 = " + (b1 >> 2));
			}
			byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2)
					: (byte) ((b1) >> 2 ^ 0xc0);
			encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[k << 4];
			encodedData[encodedIndex++] = PAD;
			encodedData[encodedIndex++] = PAD;
		} else if (fewerThan24bits == SIXTEENBIT) {
			b1 = binaryData[dataIndex];
			b2 = binaryData[dataIndex + 1];
			l = (byte) (b2 & 0x0f);
			k = (byte) (b1 & 0x03);

			byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2)
					: (byte) ((b1) >> 2 ^ 0xc0);
			byte val2 = ((b2 & SIGN) == 0) ? (byte) (b2 >> 4)
					: (byte) ((b2) >> 4 ^ 0xf0);

			encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[val2 | (k << 4)];
			encodedData[encodedIndex++] = lookUpBase64Alphabet[l << 2];
			encodedData[encodedIndex++] = PAD;
		}

		return new String(encodedData);
	}
	public static byte[] decode(String encoded) {

		if (encoded == null) {
			return null;
		}

		char[] base64Data = encoded.toCharArray();
		// remove white spaces
		int len = removeWhiteSpace(base64Data);

		if (len % FOURBYTE != 0) {
			return null;// should be divisible by four
		}

		int numberQuadruple = (len / FOURBYTE);

		if (numberQuadruple == 0) {
			return new byte[0];
		}

		byte decodedData[] = null;
		byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
		char d1 = 0, d2 = 0, d3 = 0, d4 = 0;

		int i = 0;
		int encodedIndex = 0;
		int dataIndex = 0;
		decodedData = new byte[(numberQuadruple) * 3];

		for (; i < numberQuadruple - 1; i++) {

			if (!isData((d1 = base64Data[dataIndex++]))
					|| !isData((d2 = base64Data[dataIndex++]))
					|| !isData((d3 = base64Data[dataIndex++]))
					|| !isData((d4 = base64Data[dataIndex++]))) {
				return null;
			}// if found "no data" just return null

			b1 = base64Alphabet[d1];
			b2 = base64Alphabet[d2];
			b3 = base64Alphabet[d3];
			b4 = base64Alphabet[d4];

			decodedData[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
			decodedData[encodedIndex++] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
			decodedData[encodedIndex++] = (byte) (b3 << 6 | b4);
		}

		if (!isData((d1 = base64Data[dataIndex++]))
				|| !isData((d2 = base64Data[dataIndex++]))) {
			return null;// if found "no data" just return null
		}

		b1 = base64Alphabet[d1];
		b2 = base64Alphabet[d2];

		d3 = base64Data[dataIndex++];
		d4 = base64Data[dataIndex++];
		if (!isData((d3)) || !isData((d4))) {// Check if they are PAD characters
			if (isPad(d3) && isPad(d4)) {
				if ((b2 & 0xf) != 0)// last 4 bits should be zero
				{
					return null;
				}
				byte[] tmp = new byte[i * 3 + 1];
				System.arraycopy(decodedData, 0, tmp, 0, i * 3);
				tmp[encodedIndex] = (byte) (b1 << 2 | b2 >> 4);
				return tmp;
			} else if (!isPad(d3) && isPad(d4)) {
				b3 = base64Alphabet[d3];
				if ((b3 & 0x3) != 0)// last 2 bits should be zero
				{
					return null;
				}
				byte[] tmp = new byte[i * 3 + 2];
				System.arraycopy(decodedData, 0, tmp, 0, i * 3);
				tmp[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
				tmp[encodedIndex] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
				return tmp;
			} else {
				return null;
			}
		} else { // No PAD e.g 3cQl
			b3 = base64Alphabet[d3];
			b4 = base64Alphabet[d4];
			decodedData[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
			decodedData[encodedIndex++] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
			decodedData[encodedIndex++] = (byte) (b3 << 6 | b4);

		}

		return decodedData;
	}
	
	
	public void destory() {
	};
	
	class Result {		
		private String mResult;		
		String resultStatus = null;
		String memo = null;
		String result = null;
		boolean isSignOk = false;

		public Result(String result) {
			this.mResult = result;
		}

		
		public  String getResult() {
			System.out.println(" getResult()");
			String src = mResult.replace("{", "");
			src = src.replace("}", "");
			return getContent(src, "memo=", ";result");
		}

		public  void parseResult() {
			
			try {
				String src = mResult.replace("{", "");
				src = src.replace("}", "");
				String rs = getContent(src, "resultStatus=", ";memo");
				if (sResultStatus.containsKey(rs)) {
					resultStatus = sResultStatus.get(rs);
				} else {
					resultStatus = "其他错误";
				}
				resultStatus += "(" + rs + ")";

				memo = getContent(src, "memo=", ";result");
				result = getContent(src, "result=", null);
				isSignOk = checkSign(result);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		private  boolean checkSign(String result) {
			boolean retVal = false;
			try {
				JSONObject json = string2JSON(result, "&");

				int pos = result.indexOf("&sign_type=");
				String signContent = result.substring(0, pos);

				String signType = json.getString("sign_type");
				signType = signType.replace("\"", "");

				String sign = json.getString("sign");
				sign = sign.replace("\"", "");

				if (signType.equalsIgnoreCase("RSA")) {
					retVal = doCheck(signContent, sign, PUBLIC);
				}
			} catch (Exception e) {
				e.printStackTrace();
				Log.i("Result", "Exception =" + e);
			}
			Log.i("Result", "checkSign =" + retVal);
			return retVal;
		}

		public  JSONObject string2JSON(String src, String split) {
			JSONObject json = new JSONObject();

			try {
				String[] arr = src.split(split);
				for (int i = 0; i < arr.length; i++) {
					String[] arrKey = arr[i].split("=");
					json.put(arrKey[0], arr[i].substring(arrKey[0].length() + 1));
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

			return json;
		}

		private  String getContent(String src, String startTag, String endTag) {
			String content = src;
			int start = src.indexOf(startTag);
			start += startTag.length();

			try {
				if (endTag != null) {
					int end = src.indexOf(endTag);
					content = src.substring(start, end);
				} else {
					content = src.substring(start);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

			return content;
		}
	}

}

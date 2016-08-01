package my.global;
public interface MSocketObserver {
	public abstract void OnConnect();
	public abstract void OnStart(byte[] str,int len);
	public abstract void OnReceive(byte[] str,int len);
	public abstract void OnFinish();
	public abstract void OnTimeOut();
}

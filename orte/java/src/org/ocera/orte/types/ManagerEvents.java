package org.ocera.orte.types;

public class ManagerEvents extends DomainEvents {

  public ManagerEvents()
  {
    super();
  }

  public void onRegFail() {}

  public void onMgrNew(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrNew()'..           *");
  	System.out.println(":j: *************************************************");
        appInfo.printWithLegend();
  }

  public void onMgrDelete(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrDelete()'..        *");
  	System.out.println(":j: *************************************************");
        appInfo.printWithLegend();
  }

  public void onAppRemoteNew(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
  	appInfo.printWithLegend();
  }

  public void onAppDelete(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppDelete()'..        *");
  	System.out.println(":j: *************************************************");
        appInfo.printWithLegend();
  }

  public void onPubRemoteNew(AppInfo appInfo, PubInfo pubInfo) {}

  public void onPubRemoteChanged(AppInfo appInfo, PubInfo pubInfo) {}

  public void onPubDelete(AppInfo appInfo, PubInfo pubInfo) {}

  public void onSubRemoteNew(AppInfo appInfo, SubInfo subInfo) {}

  public void onSubRemoteChanged(AppInfo appInfo, SubInfo subInfo) {}

  public void onSubDelete(AppInfo appInfo, SubInfo subInfo) {}

}


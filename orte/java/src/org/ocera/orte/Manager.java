package org.ocera.orte;

import org.ocera.orte.types.*;

public class Manager {
	
	private DomainMgr dmgr;
        private ManagerEvents events = new ManagerEvents();
        
        public Manager() {
            dmgr = new DomainMgr(ORTEConstant.ORTE_DEFAULT_DOMAIN,
                                                     null,
                                                     events,
                                                     false);
        }

        public Manager(String mgrs) {
        		DomainProp dprops = DomainProp.defaultPropsCreate();
        		dprops.setProps(mgrs);
                dmgr = new DomainMgr(ORTEConstant.ORTE_DEFAULT_DOMAIN,
                                                         dprops,
                                                         events,
                                                         false);
        }
	
	public Manager(String[] mgrs) {
		DomainProp dprops = DomainProp.defaultPropsCreate();
		dprops.setProps(mgrs);
		
		dmgr = new DomainMgr(ORTEConstant.ORTE_DEFAULT_DOMAIN,
							 dprops,
							 events,
							 false);
	}

        public boolean destroy() {
                return dmgr.destroy();
        }
}

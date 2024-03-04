#![allow(unused_imports)]
#![allow(missing_docs)]

use kernel::prelude::*;

module! {
    type: DriverAndInterrupts,
    name: "driver_and_interrupts",
    author: "Jolan Rathelot <jrathelo@student.42nice.fr>",
    description: "driver_and_interrupts 42 project",
    license: "GPL",
}

struct DriverAndInterrupts { }

impl kernel::Module for DriverAndInterrupts {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("Drivers and Interrupts (Init)");

        Ok(DriverAndInterrupts {})
    }
}

impl Drop for DriverAndInterrupts {
    fn drop(&mut self) {
        pr_info!("Drivers and Interrupts (exit)")
    }
}

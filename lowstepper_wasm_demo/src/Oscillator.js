export default class Oscillator {
  constructor(audioCtx, destinationNode) {
      this.audioCtx = audioCtx;
      this.oscNode = audioCtx.createOscillator();
      this.oscNode.type = 'sawtooth';

      // We use a gain node to "start" and "stop" the track
      // This is basically achieved by muting and unmuting the gain node that sits
      // infront of the oscillator.
      this.gainNode = audioCtx.createGain();
      this.oscNode.connect(this.gainNode);
      this.gainNode.connect(destinationNode);

      this.mute();
      this.oscNode.start();
  }

  mute() {
      this.gainNode.gain.setValueAtTime(0, this.audioCtx.currentTime);
  }

  unmute() {
      this.gainNode.gain.setValueAtTime(1, this.audioCtx.currentTime);
  }

  playFrequency(hertz) {
      this.oscNode.frequency.setValueAtTime(hertz, this.audioCtx.currentTime); // value in hertz
      this.unmute();
  }

  // just a nice alias, you logically don't mute an oscillator.
  stop() {
      this.mute();
  }
}